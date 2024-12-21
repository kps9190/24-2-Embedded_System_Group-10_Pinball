#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>
#include <wiringPiI2C.h>
#include <ao/ao.h>
#include <mpg123.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <pwd.h>




/* 스피커 */
#define AUDIO_FILE "ball.mp3"



volatile int reset_audio_flag = 0; // 스피커 리셋 플래그
pthread_mutex_t audio_mutex;       // 스피커 플래그 뮤텍스
pthread_mutex_t gpio_mutex;
/* lcd */
#define LCD_ADDR 0x3E // LCD I2C Address
#define RGB_ADDR 0x62 // RGB I2C Address

int fd_lcd;
int fd_rgb;



/* 초음파 센서 핀 번호 */
#define TRIG_PIN 12
#define ECHO_PIN 13

/*점수 깍이는 초음파 센서 핀 번호*/
#define TRIG_PIN_M 8
#define ECHO_PIN_M 25


/* 버튼 핀 번호 */
//#define BUTTON_PIN 14 // 일반 버튼 핀
#define RESET_BUTTON_PIN 27 // 리셋 버튼 핀

/* 서보 모터 핀 번호 */

//#define SERVO 18 //서보 모터 핀 21 이였음 , 반드시 13,18,19,12만 사용(PWN 사용시)

/* 부저 핀 번호*/
#define BUZZER 16

/* 전등 모듈 센서 */
#define LED 23
int ledflag = 0;

/*진동 모듈 센서*/
#define VIBRATION_SENSOR_PIN 21



/* 점수 및 뮤텍스 */
volatile int score = 0; // 점수 저장
pthread_mutex_t score_mutex; // 점수 뮤텍스

//함수 선언
void init();
void lcd_init(int fd_lcd, int fd_rgb, int r, int g, int b);
void lcd_clear(int fd_lcd);
void lcd_print(int fd_lcd, int score);

int init_audio();
void* play_audio_thread(void* arg);
void cleanup_audio();
void *sonar(void *arg);
void sonar_read();
void sonar_read_m();
void* execute_survo(void* arg);
void myTone();
void led();

//초기화
void init() {
    wiringPiSetupGpio();

    //lcd 초기화

    fd_lcd = wiringPiI2CSetup(LCD_ADDR);
    fd_rgb = wiringPiI2CSetup(RGB_ADDR);

    lcd_init(fd_lcd, fd_rgb, 0, 0, 255);//lcd & rgb 초기화

    lcd_print(fd_lcd, 0);//

    // 초음파 센서 핀 모드 설정
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW); // 트리거 핀 초기화

    // 서보 모터 초기화
    //init_servo();

    // 부저 초기화
    softToneCreate(BUZZER);

    //진동 초기화
    pinMode(VIBRATION_SENSOR_PIN, INPUT);

    //led 초기화
    pinMode(LED, OUTPUT);

    //reset 초기화
    pinMode(RESET_BUTTON_PIN, INPUT);

    // music
    //  if (init_audio() != 0) {
    //     return; // Initialization failed
    // }

    //  뮤텍스 초기화
    pthread_mutex_init(&score_mutex, NULL);
    pthread_mutex_init(&gpio_mutex, NULL);
}

void lcd_init(int fd_lcd, int fd_rgb, int r, int g, int b) {

    // Initialize RGB
    wiringPiI2CWriteReg8(fd_rgb, 0x00, 0x00); // Initialization

    wiringPiI2CWriteReg8(fd_rgb, 0x01, 0x00); // Initialization
    wiringPiI2CWriteReg8(fd_rgb, 0x08, 0xAA); // RGB activation

    wiringPiI2CWriteReg8(fd_rgb, 0x04, r); // RED
    wiringPiI2CWriteReg8(fd_rgb, 0x03, g); // GREEN
    wiringPiI2CWriteReg8(fd_rgb, 0x02, b); // BLUE

    // Initialize LCD
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x38); // Function set: 2 lines
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x39); // Function set: Extend instruction set
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x14); // OSC frequency
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x70); // Contrast setting
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x56); // Power/ICON control
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x6C); // Follower control
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x0C); // Display ON
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x01); // Clear display
    usleep(2000);
}

void lcd_clear(int fd_lcd) {
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x01);
    delay(2);
}

void lcd_print(int fd_lcd, int score) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "score: %d", score);
    
    lcd_clear(fd_lcd);

    // Print the formatted string to the LCD
    const char* str = buffer;
    while (*str) {
                wiringPiI2CWriteReg8(fd_lcd, 0x40, *str++);
        
        usleep(500);
    }
}

//스피커

pid_t play_mp3_pid = -1; 
void play_mp3() {
    
    if (play_mp3_pid > 0) {
        printf("Stopping currently running play_mp3 process (PID: %d)...\n", play_mp3_pid);
        if (kill(play_mp3_pid, SIGTERM) == -1) {  // 
            perror("Failed to terminate existing play_mp3 process");
        }
        waitpid(play_mp3_pid, NULL, 0);  // 
        play_mp3_pid = -1;
    }

    pid_t pid = fork();

    if (pid == 0) {  
        setenv("XDG_RUNTIME_DIR", "/run/user/1000", 1); 
        setenv("PATH", "/usr/bin:/bin:/usr/sbin:/sbin", 1);

        //
        if (geteuid() == 0) {
            struct passwd *pw = getpwnam("pi");
            if (!pw) {
                perror("Failed to get user 'pi'");
                exit(EXIT_FAILURE);
            }
            if (setuid(pw->pw_uid) == -1) {
                perror("Failed to drop privileges");
                exit(EXIT_FAILURE);
            }
        }

        // ./play_mp3 
        execl("./play_mp3", "play_mp3", NULL);
        perror("Failed to execute ./play_mp3");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {  
        play_mp3_pid = pid;  
        printf("Started play_mp3 process (PID: %d)\n", play_mp3_pid);
    } else {
        perror("Failed to fork");
    }
}


int testcnt=0;
int power_error=0;
// 거리 측정
void sonar_read() {
    power_error=0;
    pthread_mutex_lock(&gpio_mutex);
    //초음파 신호 발사 
    //printf("%d\n",testcnt++);
    digitalWrite(TRIG_PIN, HIGH);
    usleep(20); // 10 마이크로초 대기
    digitalWrite(TRIG_PIN, LOW);

     long timeout = micros() + 20000; // 20ms 
    while (digitalRead(ECHO_PIN) == LOW && micros() < timeout);

    if (micros() >= timeout) {
        printf("Timeout waiting for ECHO_PIN to go HIGH\n");
        power_error=1;
        //return;
    }

    long start_time = micros();
    timeout = micros() + 20000; // 20ms
    while (digitalRead(ECHO_PIN) == HIGH && micros() < timeout);

    if (micros() >= timeout) {
        printf("Timeout waiting for ECHO_PIN to go LOW\n");
        power_error=1;
        //return;
    }

    long travel_time = micros() - start_time;

    int distance = travel_time / 58; // cm

    printf("sonar read %d\n", distance);

    if(power_error==1) {
        distance = 100;
        printf("power error\n");
    }

    if(distance<=2) distance = 100;

    pthread_mutex_unlock(&gpio_mutex);
    if (distance <= 6) { // 5cm 이내에 물체가 인식되면

        pthread_mutex_lock(&score_mutex); // 뮤텍스 잠금
        score++; // 점수 증가
        lcd_print(fd_lcd, score);
        printf("Score: %d\n", score);//밖으로 빼도 되긴함
        pthread_mutex_unlock(&score_mutex); // 뮤텍스 잠금 해제

        
        led();
        myTone();

    }
}

// 초음파 센서
void *sonar(void *arg) {
    int dynamic_delay = 3000; //3s
    int save_score;
    while (1) {
        save_score = score;
        sonar_read();
        if(save_score == score)//ball not pass 50ms
            dynamic_delay =50;
        else
            dynamic_delay = 1000; //ball pass 3s
        delay(dynamic_delay);
    }
    return NULL;
}



//부저 소리
void myTone() {
    softToneWrite(BUZZER, 440);
    delay(500);
    softToneWrite(BUZZER, 0);
}

//리셋 버튼 
void *reset(void *arg) {
    //static pthread_t audioThread;

    while(1) {
        if (digitalRead(RESET_BUTTON_PIN) == LOW) {
            printf("Reset button pressed\n");
            play_mp3();
        

            pthread_mutex_lock(&score_mutex);
            score = 0; // 점수 리셋
            lcd_print(fd_lcd, score);
            pthread_mutex_unlock(&score_mutex);

            printf("reset\n");
            delay(500); // 버튼이 눌린 동안 대기
        }
        delay(50);
    }
}

void led() {

    if (ledflag == 0)
    {
        digitalWrite(LED, HIGH);
        ledflag = 1;
    }
    else {
        digitalWrite(LED, LOW);
        ledflag = 0;
    }
    
}


void* execute_survo(void* arg) {

    //rsa error 
    int ret = system("sshpass -p 'qwerty' ssh -o StrictHostKeyChecking=no pi@192.168.43.106 'sudo /home/pi/survo/survo'");      //ip는 경우에 따라 바뀜
    if (ret != 0) {
        printf("Error: Failed to execute survo (return code: %d)\n", ret);
    } else {
        printf("survo executed successfully.\n");
    }
    //survo terminate
    //system("ssh pi@192.168.0.26 'pkill -f /home/pi/survo/survo'");
    return NULL;
}

// 메인 함수
int main() {
    init();
    play_mp3();
    
    

    pthread_t sonarThread, buttonThread, resetThread, servoThread; //audioThread;

    // Create threads
    pthread_create(&sonarThread, NULL, sonar, NULL);
    pthread_create(&resetThread, NULL, reset, NULL);
    //pthread_create(&audioThread, NULL, audio_thread_func, NULL); // Start audio thread

    // Join threads (this ensures the program waits for threads to complete before terminating)
    pthread_join(sonarThread, NULL);
    pthread_join(servoThread, NULL);
    pthread_join(resetThread, NULL);
    //pthread_join(audioThread, NULL); // Wait for the audio thread to finish

    // Cleanup
    pthread_mutex_destroy(&score_mutex);
    pthread_mutex_destroy(&audio_mutex);

    //cleanup_audio();  // Release SDL audio resources

    return 0;
}
