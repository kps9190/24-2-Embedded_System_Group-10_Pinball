#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <softTone.h>
#include <wiringPiI2C.h>

/* lcd */
#define LCD_ADDR 0x3E
int lcd;
static const char* I2C_DEV = "/dev/i2c-1";

/* 초음파 센서 핀 번호 */
#define TRIG_PIN 12
#define ECHO_PIN 13

/* 버튼 핀 번호 */
#define BUTTON_PIN 14 // 일반 버튼 핀
#define RESET_BUTTON_PIN 15 // 리셋 버튼 핀
#define LEFT_FLIPPER_BUTTON_PIN 26//16 // 왼쪽 플리퍼 버튼
#define RIGHT_FLIPPER_BUTTON_PIN 17 // 오른쪽 플리퍼 버튼

/* 서보 모터 핀 번호 */

#define LEFT_FLIPPER 19 // 왼쪽 플리퍼 서보 핀
#define RIGHT_FLIPPER 20 // 오른쪽 플리퍼 서보 핀
#define SERVO 18 //서보 모터 핀 21 이였음 , 반드시 13,18,19,12만 사용(PWN 사용시)

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
void init_lcd();
void lcd_print(const char *str);
void display_lcd_number(int num);

void init_servo();

void *sonar(void *arg);
void sonar_read();

void *servo(void *arg);
void rotate_Servo(float angle);

void *lcd_segment(void *arg);

void *button(void *arg);
void *vibrationSensor(void *arg);

void myTone();
void led();

//초기화
void init() {
    wiringPiSetupGpio();

    init_lcd();

    // 초음파 센서 핀 모드 설정
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    digitalWrite(TRIG_PIN, LOW); // 트리거 핀 초기화

    // 서보 모터 초기화
    init_servo();

    // 부저 초기화
    softToneCreate(BUZZER);

    //진동 초기화
    pinMode(VIBRATION_SENSOR_PIN, INPUT);

    //  뮤텍스 초기화
    pthread_mutex_init(&score_mutex, NULL);
}

void init_lcd() {
    lcd = wiringPiI2CSetupInterface(I2C_DEV, LCD_ADDR);
    if (lcd == -1) {
        printf("LCD 초기화 실패!\n");
        return;
    }

    wiringPiI2CWriteReg8(lcd, 0x00, 0x38); // Function set
    wiringPiI2CWriteReg8(lcd, 0x00, 0x39); // Function set: extension mode
    wiringPiI2CWriteReg8(lcd, 0x00, 0x14); // Internal OSC frequency
    wiringPiI2CWriteReg8(lcd, 0x00, 0x70); // Contrast set
    wiringPiI2CWriteReg8(lcd, 0x00, 0x56); // Power/ICON control/Contrast set
    wiringPiI2CWriteReg8(lcd, 0x00, 0x6C); // Follower control
    delay(200);
    wiringPiI2CWriteReg8(lcd, 0x00, 0x38); // Function set
    wiringPiI2CWriteReg8(lcd, 0x00, 0x0C); // Display ON
    wiringPiI2CWriteReg8(lcd, 0x00, 0x01); // Clear display
    delay(2);
}

void lcd_print(const char *str) {
    while (*str) {
        wiringPiI2CWriteReg8(lcd, 0x40, *str++);
    }
}

void display_lcd_number(int num) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "Score : %d", num);
    wiringPiI2CWrite(lcd, 0x80);
    lcd_print(buffer);
}

//서보모터 초기화
void init_servo() {
    pinMode(SERVO, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);
    //pwmWrite(SERVO, 150);
}

// 거리 측정
void sonar_read() {
    digitalWrite(TRIG_PIN, HIGH);
    delay(10); // 10 마이크로초 대기
    digitalWrite(TRIG_PIN, LOW);

    while (digitalRead(ECHO_PIN) == LOW);
    long start_time = micros();
    while (digitalRead(ECHO_PIN) == HIGH);
    long travel_time = micros() - start_time;

    int distance = travel_time / 58; // cm
    //printf("%d\n", distance);
    if (distance <= 5) { // 5cm 이내에 물체가 인식되면
        pthread_mutex_lock(&score_mutex); // 뮤텍스 잠금
        score++; // 점수 증가
        led();
        myTone();
        printf("Score: %d\n", score);
        pthread_mutex_unlock(&score_mutex); // 뮤텍스 잠금 해제

    }
}

// 초음파 센서
void *sonar(void *arg) {
    while (1) {
        sonar_read();
        delay(50); // 0.1초 대기
    }
    return NULL;
}

// 회전 트랩
void rotate_Servo(float angle){
    
    if (angle > 90.0) { printf("90도가 초과되었습니다 -> 90도로 조절합니다.\n"); angle = 90.0; }
    else if (angle < -90.0) { printf("-90도 미만입니다. -> -90도로 조절합니다.\n"); angle = -90.0; }
    
    int rotate = (int)(angle * (100./90.)) + 150;  
    //printf("%.4fms\n", ((float)rotate)/100.);
    pwmWrite(SERVO, rotate);
}

// 서보 모터 계속 돌리기
void *servo(void *arg) {

    while (1) {
        rotate_Servo(0.);
        delay(1000);
        rotate_Servo(90.);
        delay(1000);
        rotate_Servo(0.);
        delay(1000);
        rotate_Servo(-90.);
        delay(1000);
    }
    return NULL;
}

//부저 소리
void myTone() {
    softToneWrite(BUZZER, 440);
    delay(500);
    softToneWrite(BUZZER, 0);
}

// lcd세그먼트 사용
void *lcd_segment(void *arg) {
    while (1) {
        pthread_mutex_lock(&score_mutex); // 뮤텍스 잠금
        display_lcd_number(score); // 현재 점수 표시
        pthread_mutex_unlock(&score_mutex); // 뮤텍스 잠금 해제
        delay(500); // 100ms 대기
        //printf("\n");
    }
    return NULL;
}

// 버튼 핸들러
void *button(void *arg) {
    while (1) {
        if (digitalRead(BUTTON_PIN) == LOW) {
            pthread_mutex_lock(&score_mutex);
            score++; // 점수 리셋
            led();
            myTone();
            printf("Score: %d\n", score);
            pthread_mutex_unlock(&score_mutex);
            delay(250); // 버튼이 눌린 동안 대기
        }
    }
}

//리셋 버튼
void *reset(void *arg) {
    while(1) {
        if (digitalRead(RESET_BUTTON_PIN) == LOW) {
            pthread_mutex_lock(&score_mutex);
            score = 0; // 점수 리셋
            pthread_mutex_unlock(&score_mutex);

            printf("reset\n");
            delay(500); // 버튼이 눌린 동안 대기
        }
        delay(50);
    }
}

void led() {
    pinMode(LED, OUTPUT);

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

void *vibrationSensor(void *arg) {
    while (1) {
        if (digitalRead(VIBRATION_SENSOR_PIN) == HIGH) { //  진동 감지
            pthread_mutex_lock(&score_mutex); // 점수 데이터 보호
            score++; //  점수 증가
            pthread_mutex_unlock(&score_mutex);

            led(); // LED 작동
            myTone(); //  부저 소리
            printf("Vibration Detected! Score: %d\n", score);
            delay(500); //  디바운스 딜레이
        }
        delay(50); //   짧은 대기 시간
    }
    return NULL;
}


// 메인 함수
int main() {
    init();

    pthread_t sonarThread, servoThread, segmentThread, buttonThread, resetThread, filperThread, vibrationThread;//, myToneThread;

    // 스레드 생성
    pthread_create(&sonarThread, NULL, sonar, NULL);
    pthread_create(&servoThread, NULL, servo, NULL);
    pthread_create(&segmentThread, NULL, lcd_segment, NULL);
    pthread_create(&buttonThread, NULL, button, NULL);
    //pthread_create(&resetThread, NULL, reset, NULL);
    pthread_create(&vibrationThread, NULL, vibrationSensor, NULL);

    // 다른 작업도 수행
    pthread_join(sonarThread, NULL);
    pthread_join(servoThread, NULL);
    pthread_join(segmentThread, NULL);
    pthread_join(buttonThread, NULL);
    pthread_join(resetThread, NULL);
    pthread_join(vibrationThread, NULL);

    // 뮤텍스 자원 해제
    pthread_mutex_destroy(&score_mutex);

    return 0;
}