#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>

//모터 드라이버
//오른쪽 바퀴
#define MOTOR_RIN1 17
#define MOTOR_RIN2 27
#define SPEEDR 18
//왼쪽 바퀴
#define MOTOR_LIN1  22 
#define MOTOR_LIN2  23
#define SPEEDL 24

//라인트레이서
#define LEFT 5
#define RIGHT 6

//장애물 센서
#define TRIG 25
#define ECHO 8


//인식률을 위한 딜레이
#define delay_front 50
#define delay_end 50

#define default_speed 55

void setup();
void loop();
void export_gpio(int gpio);
void set_gpio_direction(int gpio, const char* direction);
void write_gpio(int gpio, int value);
void set_pwm(int gpio, int frquency, int duty_cycle);

void goStr();
void goBack();
void turnR();
void turnL();

int main() {
    setup();
    while (1) {
        loop();
    }
    

    // 전진
    printf("Moving forward\n");
    write_gpio(MOTOR_RIN1, 1);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 1);
    write_gpio(MOTOR_LIN2, 0);

    sleep(5);

    // 후진
    printf("Moving backward\n");
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 1);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 1);

    sleep(5);

    // 정지
    printf("Stopping\n");
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 0);

    return 0;
}

void setup()
{
    // GPIO 핀 export 및 방향 설정
    export_gpio(MOTOR_RIN1);
    export_gpio(MOTOR_RIN2);
    export_gpio(MOTOR_LIN1);
    export_gpio(MOTOR_LIN2);
    export_gpio(SPEEDR);
    export_gpio(SPEEDL);

    set_gpio_direction(MOTOR_RIN1, "out");
    set_gpio_direction(MOTOR_RIN2, "out");
    set_gpio_direction(MOTOR_LIN1, "out");
    set_gpio_direction(MOTOR_LIN2, "out");
    set_gpio_direction(SPEEDR, "out");
    set_gpio_direction(SPEEDL, "out");

    // PWM 설정 (주파수 1kHz, 듀티 사이클 50%)
    set_pwm(PWM_PIN, 1000, 50);
}

void loop()
{
}

// GPIO export 함수
void export_gpio(int gpio) {
    char buffer[64];
    int len = snprintf(buffer, sizeof(buffer), "%d", gpio);
    int fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
        perror("Failed to export GPIO");
        exit(1);
    }
    write(fd, buffer, len);
    close(fd);
}

// GPIO 방향 설정 함수
void set_gpio_direction(int gpio, const char* direction) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", gpio);
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to set GPIO direction");
        exit(1);
    }
    write(fd, direction, strlen(direction));
    close(fd);
}

// GPIO에 값 쓰기 함수
void write_gpio(int gpio, int value) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", gpio);
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to write GPIO value");
        exit(1);
    }
    if (value)
        write(fd, "1", 1);
    else
        write(fd, "0", 1);
    close(fd);
}

// PWM을 설정하는 함수
void set_pwm(int gpio, int frequency, int duty_cycle) {
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/enable", gpio);
    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open PWM enable");
        exit(1);
    }
    write(fd, "0", 1);  // Disable PWM before setting

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/period", gpio);
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open PWM period");
        exit(1);
    }
    int period = 1000000000 / frequency; // 주기 (나노초 단위)
    dprintf(fd, "%d", period);  // Period 설정
    close(fd);

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", gpio);
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open PWM duty_cycle");
        exit(1);
    }
    int duty = (period * duty_cycle) / 100;  // 듀티 사이클 설정
    dprintf(fd, "%d", duty);  // Duty Cycle 설정
    close(fd);

    // Enable PWM
    write(fd, "1", 1);  // Enable PWM
    close(fd);
}