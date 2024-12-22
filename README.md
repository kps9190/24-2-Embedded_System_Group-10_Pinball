# Pinball
임베디드 시스템 팀 프로젝트 10조 Pinball과 관련된 GitHub 저장소입니다.



핀볼 프로그램은 크게 소프트웨어와 하드웨어 두 부분으로 나뉩니다.

먼저 하드웨어 부분의 경우, 이 프로젝트를 위해 사용된 센서의 경우 SR04 초음파센서 1개, LED 1개, 서보모터 3개, 블루투스 스피커,택트 스위치 1개, 1602 LCD 1개, 수동 버저 1개입니다. 
총 2대의 라즈베리파이를 사용하였으며, 두 대 모두 학교에서 제공하는 키트에  제공되는 GPIO 확장 키트를 장착하였습니다.
그 외 게임판을 위해서 고무줄 여러개, 장애물과 플리퍼를 구성하기 위한 우드락, 나무젓가락 여러개와 택배상자를 사용하였으며, 게임용 공으로 가벼운 구슬을 사용하였습니다.

소프트웨어적인 부분으로는 시작점이자 게임의 대부분 기능이 담긴 main.c코드와 저전력 문제와 선 부족 문제로 인해 다른 Pi에서 구동되도록 별도로 분리시킨 servo.c로 구성되어 있습니다.



게임 방식은 다음과 같습니다.

1. 발사대에 있는 나무젓가락을 뒤로 당긴 후 놓아서 공을 발사합니다.
2. 발사된 공이 게임판 좌측에 있는 초음파 센서를 지날때마다 점수가 오릅니다.
3. 공이 플리퍼 아래로 떨어지면 게임이 종료됩니다.
4. 노란색 택트 스위치를 눌러서 점수를 초기화하고 음악을 처음부터 다시 재생합니다.


게임이 다소 단조롭게 구성이 되어있다 느끼실 수 있습니다. 그 이유는 다음과 같습니다.
1. 여러 센서를 사용하려 했으나 게임판의 크기에 대해서 어느정도 현실적인 고려를 할 수 밖에 없었습니다. 너무 큰 판은 저희가 보관하는 부분에 있어서 애로사항이 컸습니다.
2. 충격 센서를 사용해서 특정 충격 부분에서 충격(이 게임의 경우 공이 부딪히는 경우)감지시 점수가 오르도록 해 보았으나 외부적인 충격에 게임에 개입되는 부분이 있어왔고, 이것을 구분시키도록 하는 것이 불가능해서 제외했습니다.
3. 초기 계획은 초음파 센서 3개를 사용하는 것 이였습니다. 이를 통해서 점수 획득가능한 범위를 넓히려 했으나, 초음파 센서 특성상 음파로 공의 이동을 감지하기 때문에 1번에 제시된 게임 판 문제도 있었고, 의도치않게 발사된 음파가 벽을 타고 반사되면서 인식이 되어서 게임 점수가 변하는 문제가 있어와 이를 관리하는것이 어려워 1개로 줄이게 되었습니다. 다만 1개로 줄이고 나름의 신호무시절차를 짜 봤으나 특성 자체를 무시할 순 없어서 점수 오작동 부분은 여전히 존재합니다.

공을 발사하는 매커니즘과 플리퍼의 경우 나무젓과락과 고무줄을 이용합니다. 이 부분 역시 라즈베리파이를 사용해서 전자적인 방식으로 구현하려 했으나, 저희가 의도하는 충분한 힘을 내려면 별도의 모터 구입이 필요했는데, 생각보다 가격이 저렴하지 않아서 직접적으로 발사대를 만들게 되었습니다.

플리퍼의 경우 나무 젓가락을 잘라서 한개의 막대를 만들고 막대에 플리퍼에 끼우고, 막대를 게임 판 아래로 뚫어서 플리퍼와 게임판 바닥이 닿도록 구성하고, 게임판 뒷면에 또 다른 막대 하나를 설치하고 플리퍼를 고정하는 막대와 고무줄로 연결함으로써, 고무줄의 장력을 이용해서 플리퍼를 치고 나서 다시 원래 위치로 돌아오도록 구성하였습니다.
십자가 모양의 장애물의 경우, 10mm 우드락을 이용해서 장애물 모양을 만들고, 해당 우드락 하단면에 서보모터 날개를 부착하고 서보모터를 게임 판 하단에서 게임판으로 뚫어서 올린다음 서보모터 날개와 결합하는 방식으로 구현하였습니다.
배경음악 재생을 담당하는 블루투스 스피커의 경우, 무선연결 특성상 위치 제약이 없기 때문에 게임판 내 빈 공간에 스피커를 배치했습니다.
초음파 센서의 경우, 게임 판 왼쪽 중앙부분에 구멍을 내고 초음파센서를 끼우는 방식을 사용했습니다. 
공이 지나가는것을 감지하는 LED를 근처에 배치함으로써 점수의 출처를 쉽게 알 수 있도록 하였고, 공이 지나가는것을 감지하면 별도의 위치에 있는 수동 버저에서 소리가 나도록 구성하였습니다.
게임점수 초기화에 사용하는 버튼의 경우 게임판과는 독립된 위치에 브레드보드를 이용해서 구현하였습니다.



소프트웨어적인 구성의 경우, main 함수의 경우 LCD, 서보모터 실행, 음악, 수동버저, LED를 담당하는 함수로 구성되어 있습니다.

먼저 LCD를 담당하는 코드를 
void lcd_init(int fd_lcd, int fd_rgb, int r, int g, int b);
void lcd_clear(int fd_lcd);
void lcd_print(int fd_lcd, int score);
과 같이 구성하였고

배경음악을 담당하는 코드를
int init_audio();
void* play_audio_thread(void* arg);
void cleanup_audio();
다음과 같이 구성하였습니다.

초음파센서를 담당하는 코드를
void *sonar(void *arg);
void sonar_read();
void sonar_read_m();
다음과 같이 구현하였고,

서보모터의 경우
void* execute_survo(void* arg); 코드를 를 통해 다른 라즈베리파이로 명령을 보내 서보모터를 실행하도록 구성하였습니다.

수동버저는
void myTone();
함수를 통해 구현되고

점수 획득을 시각적을 보여주는 LED의 경우
void led();
함수를 사용하게 됩니다.


-서보모터-
서보모터의 경우 별도의 servo.c 파일로 구성해서 다른 라즈베리파이에서 담당하도록 구현하였습니다. 이는 한 대의 파이에서 구동하기에는 GPIO PIN의 부족 문제와 전력 부족 문제때문입니다.

서보모터의 실행을 담당하는 코드는 다음과 같습니다.
```c
void init_servo() {
    pinMode(SERVO, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);
}
```

표준 서보모터의 설정값인, PWM 동작모드는 마크-스페이스 모드, 듀티 사이클은 2,000에 클럭은 50Hz입니다.

서보모터의 회전 코드는 하기 내용과 같습니다.
```c
void rotate_Servo(float angle){
    if (angle > 90.0) {
        angle = 90.0;
    }
    else if (angle < -90.0) {
        angle = -90.0;
    }
    int rotate = (int)(angle * (100./90.)) + 150;
    pwmWrite(SERVO, rotate);
}
```
해당 코드를 통해 서보모터 회전 각도를 -90도에서 +90도, 총 180도 내에서만 구동되도록 하였으며, 혹시 의도치않게 범위를 넘는 경우 이를 범위 내로 재 조정하도록 하였습니다.

위 설정값을 바탕으로 게임 내에서 서보모터가 동작하는 코드는 다음과 같습니다.
```c
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
```
서보모터의 회전이 1초 간격으로 0도 -> 90도 -> 0도 -> -90도 -> 0도로 해서 한번에 90도씩 회전하도록 하였습니다.


이렇게 구동되도록 맞추어진 서보모터는,
```c
int main() {
    init();
    pthread_t servoThread; 
    pthread_create(&servoThread, NULL, servo, NULL);
    pthread_join(servoThread, NULL);
    return 0;
}
```

해당 코드를 통해 서보모터 구동을 위한 스레드를 만든후에 구동되도록 하였습니다.

이 서보모터는 
해당 서보모터는 주 라즈베리파이에서 핀볼 코드 실행시 실행되는, 아래에 있는 execute_servo코드를 실행해서
```c
void* execute_survo(void* arg) {
    int ret = system("sshpass -p 'qwerty' ssh -o StrictHostKeyChecking=no pi@192.168.43.106 'sudo /home/pi/survo/survo'");
    return NULL;
}
```
execute_servo를 통해서 동일 네트워크에 접속해있는 다른 라즈베리파이로 SSH 명령어를 보내서 다른 라즈베리파이의  /home/pi/survo/survo에 있는 코드를 실행하도록 하였습니다.
이번 시연에서는 휴대폰에서 핫스팟을 켠 후 두 대의 파이를 한대의 핫스팟에 접속시켜서 동일 네트워크로 묶은 후에 명령을 보내는 방식을 사용하였습니다.



-1602 LCD-
LCD와 라즈베리파이는 I2C 인터페이스로 연결되어 있으며, {무언갈 더 적고싶은데...}


먼저 LCD의 화면을 초기화 하는 코드입니다.
```c
void lcd_clear(int fd_lcd) {
    wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x01);
    delay(2);
}
```

LCD 초기화 명령이 실행되면, LCD에 0x01이라는, LCD의 초기화를 담당하는 명령을 보내서 화면을 초기화하게 됩니다.

이렇게 초기화 하고 난 LCD에 점수를 기록하기 위해서는 화면 출력을 담당하는 별도의 코드가 필요한데, 이는 lcd_print()에서 담당하게 됩니다.
하기 코드는 초기화 된 LCD에 화면 출력을 담당하는 코드입니다.
```c
void lcd_print(int fd_lcd, int score) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "score: %d", score);
    
    lcd_clear(fd_lcd);
    
    const char* str = buffer;
    while (*str) {
                wiringPiI2CWriteReg8(fd_lcd, 0x40, *str++);
        
        usleep(500);
    }
}
```

{화면출력이 이루어지는 방식을 설명하도록}


-배경음악-
게임이 시작되면 play_mp3()를 호출해서 배경음악이 재생되도록 하였습니다.
배경음악 재생을 담당하는 코드는 다음과 같습니다.
```c
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
```

{배경음악이 재생되는 방식을 설명하도록}
라즈베리파이 저장소에 저장된 별도의 play_mp3라는 파일을 불러들여서 재생되도록 하였습니다.



-초음파 센서-
초음파 센서의 종합적인 구동을 담당하는 코드는 다음과 같습니다.
```c
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
```
{종합적인 구동 설명}

여기서 초음파 센서에서 물체의 감지를 담 sonar_read()를 담당하는 코드는 다음과 같습니다.
```c
void sonar_read() {
    power_error=0;
    pthread_mutex_lock(&gpio_mutex);
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
```
{sonar read와 관련된 설명, 초음파센서가 어떻게 물체를 감지하는지 등등

 하기 내용은 아직 정리가 안된 부분이지만 일단 적어둠



    점수가 오르는 기준은 초음파 센서에서 5CM 이내에 물체가 인식되는지를 확인하는 방식입니다.
    만약 5CM이내에서 물체가 인식이 되는 경우(distance <= 6)

먼저 다른 프로세스에 의해서 점수가 의도치않게 변동되는 것을 막기 위해서 점수에 뮤텍스 잠금을 걸어서 다른 프로세스의 개입을 차단하도록 하였습니다.
        pthread_mutex_lock(&score_mutex); 
개입 차단후에 점수를 1점 올리고(score++)
1602 LCD 화면을 현재의 점수 값에 맞게 갱신하고 (lcd_print(fd_lcd, score)); )
점수 뮤텍스의 잠금을 해제하고 (pthread_mutex_unlock(&score_mutex);) 
LED가 점등되고 수동 버저에서 소리가 재생되도록 하였습니다. (led(); myTone();)

}



-LED-
LED의 점멸을 담당하는 코드는 다음과 같습니다.
```c
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
```
    
해당 코드를 통해서 LED의 Flag를 조절하는 방식으로 점수 획득에 대한 시각적인 효과를 제공하도록 구성하였습니다.


-수동버저-
점수 획득시 수동버저가 울리도록 담당한 코드는 다음과 같습니다.
```c
void myTone() {
    softToneWrite(BUZZER, 440);
    delay(500);
    softToneWrite(BUZZER, 0);
}
```
myTone()이 호출(점수의 상승)되는 경우, 440Hz의 소리가 재생되도록 하였습니다.



-재설정-
재설정을 담당하는 코드는 다음과 같습니다.
```c
void *reset(void *arg) {
    //static pthread_t audioThread;

    while(1) {
        if (digitalRead(RESET_BUTTON_PIN) == LOW) {
            printf("Reset button pressed\n");
            play_mp3();

            pthread_mutex_lock(&score_mutex);
            score = 0;
            lcd_print(fd_lcd, score);
            pthread_mutex_unlock(&score_mutex);

            printf("reset\n");
            delay(500);
        }
        delay(50);
    }
}
```
(digitalRead(RESET_BUTTON_PIN) == LOW)를 통해서 버튼이 눌린것을 감지하면, play_mp3();를 통해서 배경음악 재생 함수를 호출해서 배경음악이 처음부터 재생되도록 하였고,        
pthread_mutex_lock(&score_mutex);를 통해서 점수에 뮤텍스 잠금을 걸고, score = 0;을 통해서 점수를 0점으로 설정하고, lcd_print(fd_lcd, score);를 통해서 갱신 된 점수가 LCD에 표기되도록 하고 마지막으로 pthread_mutex_unlock(&score_mutex);를 통해서 점수 뮤텍스에 걸려 있던 잠금을 해제하도록 하였습니다. 
점수에 뮤텍스 잠금을 걸고, 점수를 0점으로 초기화 하고 LCD에 표시되는 점수를 현재 점수의 값(0점)에 맞게 갱신하고 점수 뮤텍스의 잠금을 해제하도록 구성하였습니다.
이를 통해서 점수가 초기화 되는 과정에서 다른 변수가 점수에 개입되는것을 막아 0점의 무결성이 유지되도록 하였습니다.



-종합-
```c
int main() {
    init();
    play_mp3();
    pthread_t sonarThread, buttonThread, resetThread, servoThread; //audioThread;
    
    pthread_create(&sonarThread, NULL, sonar, NULL);
    pthread_create(&resetThread, NULL, reset, NULL);
    pthread_join(sonarThread, NULL);
    pthread_join(servoThread, NULL);
    pthread_join(resetThread, NULL);

    
    pthread_mutex_destroy(&score_mutex);
    pthread_mutex_destroy(&audio_mutex);
    
    return 0;
}
```

Main함수 구성은 다음과 같습니다.
코드가 실행되면, 초음파 센서를 사용하는 sonar, 리셋 버튼을 처리하는 reset 같은 작업들을 실행할 각각의 스레드를 만들어서 실행합니다. 

이렇게 만든 스레드들은 동시에 동작하며 서로 간섭하지 않고 독립적으로 실행됩니다. 마지막으로 프로그램이 종료되기 전, 사용한 뮤텍스를 정리하여 프로그램이 깔끔하게 마무리되도록 합니다.






최종발표에 사용된 프레젠테이션 파일은 
https://1drv.ms/p/c/b826fcfcce360823/EfU9jbzG73ZIri-toj1JGh0BeaA6BwlzsAelqIpkgt1cMQ?e=Nrdrhc
이 링크를 통해서 보실 수 있습니다. 용량이 너무 커서 GitHub에는 첨부할 수 없었습니다.
