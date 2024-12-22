# Pinball
임베디드 시스템 팀 프로젝트 10조 Pinball과 관련된 GitHub 저장소입니다.

---

## 개요

핀볼 프로그램은 크게 **소프트웨어**와 **하드웨어** 두 부분으로 나뉩니다.

### 하드웨어 구성

- **사용된 센서 및 부품**
  - SR04 초음파 센서 1개
  - LED 1개
  - 서보모터 3개
  - 블루투스 스피커
  - 택트 스위치 1개
  - 1602 LCD 1개
  - 수동 버저 1개
    
- **라즈베리파이**: 총 2대 (GPIO 확장 키트 사용)
  
- **게임판 구성**
  - 고무줄, 우드락, 나무젓가락, 택배상자
  - 가벼운 구슬 (게임용 공)

### 소프트웨어 구성

- **Main 코드**: `main.c`
  - 게임의 대부분 기능 포함
    
- **서보모터 제어 코드**: `servo.c`
  - GPIO 핀 및 전력 부족 문제 해결을 위해 별도 분리

---

## 게임 방식

1. **공 발사**: 발사대 나무젓가락을 뒤로 당긴 후 놓아서 공 발사.
2. **점수 획득**: 초음파 센서를 지날 때마다 점수 증가.
3. **플리퍼 작동**: 물리적으로 바를 밀어 넣어 플리퍼를 작동
4. **게임 종료**: 공이 플리퍼 아래로 떨어지면 게임 종료.
5. **점수 초기화**: 노란색 택트 스위치를 눌러 점수 초기화 및 음악을 다시 재생.

---

## 하드웨어 설계

| 구성 요소      | 상세 설명                                                                 |
|----------------|------------------------------------------------------------------------|
| **발사대**      | 나무젓가락과 고무줄 사용. 충분한 힘을 제공하기 위해 전자식 대신 수동 설계.          |
| **플리퍼**      | 고무줄 장력을 이용해 나무젓가락으로 제작. 장애물과 플리퍼를 우드락 및 서보모터로 구성.  |
| **초음파 센서**  | 게임판 왼쪽 중앙에 설치. 물체 감지 시 LED 점등 및 버저 소리 발생.                   |
| **LCD 화면**    | 점수 표시를 위해 I2C 인터페이스로 연결.                                        |
| **서보모터**        | 게임을 하는 동안 서보모터를 동작하여 움직임                                  |
| **음악 재생**    | 블루투스 스피커 사용. 게임판 내 빈 공간에 배치.                                   |
| **버튼**        | 점수 초기화를 위한 독립 버튼 (브레드보드에 연결).                                  |


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

---

## 소프트웨어 구성

### 주요 함수 (test.c)

| 함수 이름              | 설명                                                                 |
|-----------------------|--------------------------------------------------------------------|
| `init()`              | 전반적인 초기화                                                        |
| `lcd_init()`          | LCD 초기화                                                        |
| `lcd_clear()`         | LCD 화면 클리어                                                  |
| `lcd_print()`         | 점수 출력                                                       |
| `play_mp3()`          | 배경음악 실행                                                    |
| `sonar_read()`        | 초음파를 이용하여 물체 감지 및 점수 처리                                   |
| `sonar()`             | 초음파 관련 스레드 3초 후에 다시 초음파 측정                                |
| `myTone()`            | 점수 획득 시 버저 음 재생                                           |
| `reset()`             | 점수 초기화 및 음악 다시 재생하는 스레드                                           |
| `led()`               | 점수 획득 시 LED 점멸등                                             |
| `execute_servo()`     | 서보모터 제어 명령 전달하는 스레드                                           |
| `main()`             | 메인 함수                                                  |

### 주요 함수 (survo.c)

| 함수 이름              | 설명                                                                 |
|-----------------------|--------------------------------------------------------------------|
| `init()`              | 전반적인 초기화                                                        |
| `init_servo()`          | 서보모터 초기화                                                        |
| `rotate_Servo()`         | 서보모터 회전                                                 |
| `servo()`         | 서보모터 스레드                                                       |
| `main()`         | 메인 함수                                                       |

---

## 주요 코드

### 서보모터 제어코드 (survo.c)

- 서보모터 회전 설정값
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

서보모터의 회전 설정값을 담당하는 코드입니다. 별도의 명령이 떨어지기 전까지 1초 간격으로 0도 -> 90도 -> 0도 -> -90도(270도) -> 0도 
이렇게 180도라는 범위 내에서 90도씩 회전하도록 하였습니다.

- PWM 설정 :
    ```c
    void init_servo() {
        printf("init servo \n");
        pinMode(SERVO, PWM_OUTPUT);
        pwmSetMode(PWM_MODE_MS);
        pwmSetRange(2000);
        pwmSetClock(192);
        //pwmWrite(SERVO, 150);
    }
  ```

    서보모터의 PWM을 담당하는 코드입니다. 마크 스페이스 모드로 실행되며, 듀티 사이클은 2,000이고, 50Hz로 구동됩니다.
  이는 표준적인 서보모터의 PWM 설정값과 동일합니다.

- 서보모터 회전 :
  ```c
    void rotate_Servo(float angle){
    
    if (angle > 90.0) { printf("90도가 초과되었습니다 -> 90도로 조절합니다.\n"); angle = 90.0; }
    else if (angle < -90.0) { printf("-90도 미만입니다. -> -90도로 조절합니다.\n"); angle = -90.0; }
    
    int rotate = (int)(angle * (100./90.)) + 150;  
    //printf("%.4fms\n", ((float)rotate)/100.);
    pwmWrite(SERVO, rotate);
    }
  ```
  서보모터의 회전을 담당하는 코드입니다. -90도(270도)에서 +90도까지 회전하도록 범위를 설정하였고, 혹시 이 범위를 넘는 회전값이 입력되는 경우 강제로 범위내의 값으로 설정하도록 하여 혹시 모를 오작동을 방지하고자 하였습니다.

---

### 초음파 제어 코드

- 초음파 스레드
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
공이 초음파 센서를 통과했는지 검사하는 코드입니다.
만약에 현재 점수값(save_score)이 score값과 일치하는 경우에는 공이 통과하지 않은 상황이라 않고 50ms 간격으로 공의 통과여부를 검사합니다.
만약 현재 점수값이 score값과 일치하지 않는 경우에는 방금 공이 통과한 상황이라 판단하고 초음파 센서의 오류 방지를 위해 통과여부가 감지되지 않는 시간을 1초로 늘립니다.
정상적인 게임 환경에서는 한번 센서를 통과한 공이 다시 해당 센서를 1초내에 통과 할 일이 없기 때문에, 자칫 발생할 수 있는 점수 오류를 막기 위함입니다.


- 초음파 제어 처리
  ```c
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
  ```
초음파 센서의 공 통과여부를 감지하는 코드입니다.
먼저 뮤텍스 잠금을 걸고, 초음파 신호를 발사하는 TRIG PIN을 HIGH로 놓은  후 10마이크로초 이후 LOW로 전환하면서 신호를 발사합니다.

그 후 초음파 수신을 담당하는 ECHO_PIN의 신호가 LOW에서 HIGH로 변화하는것을 감지하면 발사 후 다시 감지될 때 까지의 시간을 계산해서 해당 시간을 travel_time이라는 변수에 저장합니다.
이 travel_time을 58로 나누게 되면 초음파 센서에서 신호가 발사된 이후 돌아오는 거리 계산이 가능한데
만약 저희가 게임판에 설정해놓은 간격인 6cm보다 낮은 수치가 감지되는 경우 공이 통과한것으로 감지를 하되
만약 2cm이하로 감지되는 경우에는 오류라 판단합니다.
그외에는 점수 뮤텍스를 잠그고, score++를 통해 점수를 1점 올리고, LCD의 점수판을 갱신하고 점수 뮤텍스의 잠금을 해제하고, LED랑 myTone함수를 호출합니다.

이것이 계속 반복되도록 합니다.


---

### LCD 관련 코드

- LCD 초기화
  ```c
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
  ```



- 1602 LCD 화면 초기화
  ```c
      void lcd_clear(int fd_lcd) {
        wiringPiI2CWriteReg8(fd_lcd, 0x80, 0x01);
        delay(2);
    }
  ```
1602 LCD의 화면출력을 초기화하는 코드입니다. LCD변수인 fd_lcd에 초기화코드인 0x01을 입력함으로써 현재 LCD에 표시되어 있는 화면을 초기화합니다.

- LCD 화면 출력
  ```c
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
  ```
LCD에 점수를 출력하도록 하는 코드입니다. 먼저 buffer에 "score: "이라는 양식을 저장해두고, score 뒷부분에 %d라는 변수를 두고 %d값이 score를 나타내도록 합니다. 
그 뒤로 lcd_clear를 통해 화면을 초기화한 이후 buffer에서 빈 글자가 인식될 때까지 하나씩 읽고 이를 lcd에 데이터모드(0x40)로 전송합니다.

---

### 스피커 관련 코드

- 스피커
  ```c
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
  배경음악 재생을 담당하는 코드입니다.
  play_mp3_pid가 0보다 큰, 즉 현재 해당 프로세스가 실행중이면 해당 프로세스를 종료합니다.
  이는 게임이 시작할 때 초기화를 하기 위한 목적도 있고, 리셋 버튼을 눌렀을 때 배경음악을 처음부터 재생하도록 하기 위함도 있습니다.

  그 이후로 play_mp3의 프로세스를 사용자 권한으로 낮추도록 설정하는데, root권한에서 해당 프로세스가 실행되는 경우 오류가 발생했기 때문입니다.
  그러고 나서 execl("./play_mp3)를 통해서 play_mp3 코드를 실행하고, play_mp3의 Process ID, pid의 존재여부를 확인함으로써 정상적으로 실행되었음을 알립니다.

---

### 서보모터 실행 코드

- 서보모터 실행하는 코드를 제어 명령 전달
  ```c
    void* execute_survo(void* arg) {
    
        //rsa error 
        int ret = system("sshpass -p 'qwerty' ssh -o StrictHostKeyChecking=no pi@192.168.43.106 'sudo /home/pi/survo/survo'");
        if (ret != 0) {
            printf("Error: Failed to execute survo (return code: %d)\n", ret);
        } else {
            printf("survo executed successfully.\n");
        }
        //survo terminate
        //system("ssh pi@192.168.0.26 'pkill -f /home/pi/survo/survo'");
        return NULL;
    }
  ```
  GPIO PIN 문제와 전력문제로 서보모터의 구동은 다른 라즈베리파이에서 실행됩니다.
  주 라즈베리파이에서 main함수를 호출함으로써 게임을 실행되면, 해당 코드를 통해 서보모터가 연결된 라즈베리파이가 /home/pi/servo에 있는 servo라는 파일이 실행되도록 하였습니다.
  이번 시연에서는 이를 위해서 별도의 휴대전화에서 모바일 핫스팟 이라는 기능을 통해 한개의 로컬 네트워크를 생성해서 두 대의 라즈베리 파이가 해당 핫스팟에 접속되도록 함으로써 한개의 네트워크를 구축해서 해당 네트워크를 통해서 명령을 전송하도록 하였습니다.
---

### 그외 코드

- 부저 소리
  ```c
    void myTone() {
        softToneWrite(BUZZER, 440);
        delay(500);
        softToneWrite(BUZZER, 0);
    }
  ```
수동 버저를 담당하는 함수입니다. 초음파 센서에서 물체(공)를 감지하게 되면 호출되며, 440Hz의 소리를 울리도록 함으로써 점수를 획득하였단걸 청각적으로 인지할 수 있도록 하였습니다. 
  

- 리셋 버튼 관련
  ```c
    void *reset(void *arg) {
    
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
  ```
초기화 함수입니다. Reset_Button이 눌리는 경우(LOW) 호출되며, play_mp3()함수를 다시 호출해서 배경음악 재생이 처음부터 다시 시작되도록 하였고, 점수 뮤텍스에 잠금을 걸고 점수를 0점으로 설정한 다음, lcd_print 함수를 호출해서 점수를 갱신하고(0점으로 갱신) 점수 뮤텍스의 잠금을 해제합니다.
이로써 게임내에서 이루어진 모든 결과물을 초기 상태로 되돌립니다.

  - LED 관련
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
        
    }
```

LED 함수입니다. 
만약 초음파 센서에서 물체를 감지하게 되는 경우 수동 버저를 담당하는 myTone()함수와 함께 호출되어, 물체가 감지될 때마다 LED가 꺼졌다 켜졌다 하게 됩니다.

---

### 메인 함수

- survo.c 에서의 메인함수
  ```c
    int main() {
        init();
        
        pthread_t servoThread; 
        pthread_create(&servoThread, NULL, servo, NULL);
        pthread_join(servoThread, NULL);
    
        return 0;
    }
  ```

  Servo모터의 구동을 담당하는 메인 함수입니다. 서보모터의 구동을 위한 servoThread를 선언하고 만들어서, 서보모터의 구동에 대한 전반적인 역할을 담당하도록 하였습니다.

- test.c 에서의 메인 함수
  ```c
    int main() {
        init();
        play_mp3();
        
        pthread_t sonarThread, buttonThread, resetThread, servoThread;
    
        // Create threads
        pthread_create(&sonarThread, NULL, sonar, NULL);
        pthread_create(&resetThread, NULL, reset, NULL);
    
        // Join threads (this ensures the program waits for threads to complete before terminating)
        pthread_join(sonarThread, NULL);
        pthread_join(servoThread, NULL);
        pthread_join(resetThread, NULL);
    
        // Cleanup
        pthread_mutex_destroy(&score_mutex);
        pthread_mutex_destroy(&audio_mutex);
    
        return 0;
    }
  ```
  위에서 정의된 각종 코드들을 동시에 실행하는 핵심적인 메인함수입니다.
  게임이 실행되면 play_mp3()를 호출해 배경음악이 재생되도록 하였고, 게임 구동에 필요한 4개(sonar, button, reset, servo)의 스레드를 선언하고, sonarThread와 resetThread를 만듭니다. 또, 각각의 스레드들이 정상적으로 종료될 때 까지 대기하도록 하며, 게임 종료시(resetThread 실행시) 게임에 사용된 2개의 뮤텍스를 정리하도록 하였습니다.
---

## 한계 및 개선 사항

- 한계
1. **게임판 크기 제한**: 현실적인 보관 문제로 인해 게임판 크기를 제한.
2. **초음파 센서 오작동**: 반사파로 인해 점수 오작동 가능성 존재.
3. **충격 센서 사용 중단**: 외부 충격과의 구분이 어려워 제외.
4. **모터 가격 문제**: 발사대 및 플리퍼에 충분한 힘을 제공하기 위해 수동 방식을 채택.

- 개선사항
1. 서보모터의 작동 방식을 **ssh 방식 대신 UART 사용**을 사용하여서 개선해야 할 것.


---

최종 영상 : https://drive.google.com/file/d/14ocFdvG7KlYg8Bv7X7S0CzXEyx_R3Vwc/view?usp=sharing

최종발표에 사용된 프레젠테이션 파일은 
https://1drv.ms/p/c/b826fcfcce360823/EfU9jbzG73ZIri-toj1JGh0BeaA6BwlzsAelqIpkgt1cMQ?e=Nrdrhc
이 링크를 통해서 보실 수 있습니다. 용량이 너무 커서 GitHub에는 첨부할 수 없었습니다.
