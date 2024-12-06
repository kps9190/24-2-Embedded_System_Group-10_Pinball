#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <unistd.h>

#define LCD_ADDR 0x3E   // LCD I2C 주소
#define RGB_ADDR 0x62   // RGB 백라이트 I2C 주소

#define LCD_BACKLIGHT 0x08
#define ENABLE 0x04
#define CMD 0
#define DATA 1

int lcd_fd;
int rgb_fd;

// LCD 및 RGB 초기화 함수
void lcdWrite(int data, int mode);
void lcdInit();
void lcdPrint(const char *str);
void lcdSetCursor(int row, int col);
void lcdClear();
void lcdDisplayControl(int display, int cursor, int blink);
void rgbBacklightInit();

// LCD 데이터 전송 함수
void lcdWrite(int data, int mode) {
    int highNibble = data & 0xF0;
    int lowNibble = (data << 4) & 0xF0;

    wiringPiI2CWrite(lcd_fd, highNibble | mode | LCD_BACKLIGHT | ENABLE);
    usleep(500);
    wiringPiI2CWrite(lcd_fd, highNibble | mode | LCD_BACKLIGHT);
    usleep(500);

    wiringPiI2CWrite(lcd_fd, lowNibble | mode | LCD_BACKLIGHT | ENABLE);
    usleep(500);
    wiringPiI2CWrite(lcd_fd, lowNibble | mode | LCD_BACKLIGHT);
    usleep(500);
}

// RGB 백라이트 초기화 함수
void rgbBacklightInit() {
    wiringPiI2CWriteReg8(rgb_fd, 0x00, 0x00);  // 모드 설정
    wiringPiI2CWriteReg8(rgb_fd, 0x01, 0x00);  // LED 출력 활성화
    wiringPiI2CWriteReg8(rgb_fd, 0x08, 0xFF);  // Red 값
    wiringPiI2CWriteReg8(rgb_fd, 0x09, 0xFF);  // Green 값
    wiringPiI2CWriteReg8(rgb_fd, 0x0A, 0xFF);  // Blue 값
}

// LCD 초기화 함수
void lcdInit() {
    rgbBacklightInit(); // RGB 백라이트 초기화
    lcdWrite(0x03, CMD);
    lcdWrite(0x03, CMD);
    lcdWrite(0x03, CMD);
    lcdWrite(0x02, CMD); // 4비트 모드 설정 
    lcdWrite(0x28, CMD); // 2행, 5x8 폰트 설정
    lcdDisplayControl(1, 0, 0); // 화면 ON, 커서 OFF, 깜박임 OFF
    lcdClear();
    lcdWrite(0x06, CMD); // 자동 커서 이동 설정
}

// 문자열 출력 함수
void lcdPrint(const char *str) {
    while (*str) {
        lcdWrite(*str++, DATA);
    }
}

// 커서 위치 설정 함수
void lcdSetCursor(int row, int col) {
    int addr = (row == 0 ? 0x80 : 0xC0) + col;
    lcdWrite(addr, CMD);
}

// LCD 화면 클리어 함수
void lcdClear() {
    lcdWrite(0x01, CMD);
    usleep(2000);
}

// LCD 화면, 커서, 깜박임 설정 함수
void lcdDisplayControl(int display, int cursor, int blink) {
    int command = 0x08;
    if (display) command |= 0x04;
    if (cursor) command |= 0x02;
    if (blink) command |= 0x01;
    lcdWrite(command, CMD);
}
/*
// 메인 함수
int main() {
    // I2C 장치 초기화
    lcd_fd = wiringPiI2CSetup(LCD_ADDR);
    if (lcd_fd == -1) {
        printf("I2C LCD 초기화 실패\n");
        return -1;
    }

    rgb_fd = wiringPiI2CSetup(RGB_ADDR);
    if (rgb_fd == -1) {
        printf("I2C RGB 백라이트 초기화 실패\n");
        return -1;
    }

    // GPIO 초기화
    if (wiringPiSetupGpio() == -1) {
        printf("GPIO 초기화 실패\n");
        return -1;
    }

    // LCD 초기화
    lcdInit();

    // 매세지 출력 
    lcdSetCursor(0, 0); // 첫 번째 줄 첫번째 칸
    lcdPrint("Hello, Raspberry!");
    lcdSetCursor(1, 0); // 두 번째 줄 첫 번째 칸
    lcdPrint("I2C LCD Test");

    return 0;
}

*/

int main() {
    rgb_fd = wiringPiI2CSetup(RGB_ADDR);
    if (rgb_fd == -1) {
        printf("RGB I2C 초기화 ����\n");
        return -1;
    }

    wiringPiI2CWriteReg8(rgb_fd, 0x08, 0xFF);  // Red �ִ�
    wiringPiI2CWriteReg8(rgb_fd, 0x09, 0x00);  // Green ����
    wiringPiI2CWriteReg8(rgb_fd, 0x0A, 0x00);  // Blue ����

    printf("RGB 백라이트 �׽�Ʈ �Ϸ�\n");
    return 0;
}

