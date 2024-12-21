#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <wiringPi.h>
#include <softPwm.h>


/* 서보 모터 핀 번호 */

#define SERVO 18 //서보 모터 핀 21 이였음 , 반드시 13,18,19,12만 사용(PWN 사용시)


//함수 선언
void init();
void init_servo();


//초기화
void init() {
    wiringPiSetupGpio();
    // 서보 모터 초기화
    init_servo();
}

//서보모터 초기화
void init_servo() {
    printf("init servo \n");
    pinMode(SERVO, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(2000);
    pwmSetClock(192);
    //pwmWrite(SERVO, 150);
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

// 메인 함수
int main() {
    init();
    
    pthread_t servoThread; 
    
    pthread_create(&servoThread, NULL, servo, NULL);

    pthread_join(servoThread, NULL);

    return 0;
}
