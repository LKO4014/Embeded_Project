#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//���� ����̹�
//������ ����
#define MOTOR_RIN1 17
#define MOTOR_RIN2 27
#define SPEEDR 18
//���� ����
#define MOTOR_LIN1  22 
#define MOTOR_LIN2  23
#define SPEEDL 24

//����Ʈ���̼�
#define LEFT 5
#define RIGHT 6

//��ֹ� ����
#define TRIG 25
#define ECHO 8


//�νķ��� ���� ������
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
void stop();
void turnR();
void turnL();

int goLeft, goRight;

int main() {
    setup();
    while (1) {
        loop();
        sleep(10);
     }

    return 0;
}

void setup()
{
    // GPIO �� export �� ���� ����
    export_gpio(MOTOR_RIN1);
    export_gpio(MOTOR_RIN2);
    export_gpio(MOTOR_LIN1);
    export_gpio(MOTOR_LIN2);
    export_gpio(SPEEDR);
    export_gpio(SPEEDL);
    
    export_gpio(LEFT);
    export_gpio(RIGHT);
    export_gpio(TRIG);
    export_gpio(ECHO);

    set_gpio_direction(MOTOR_RIN1, "out");
    set_gpio_direction(MOTOR_RIN2, "out");
    set_gpio_direction(MOTOR_LIN1, "out");
    set_gpio_direction(MOTOR_LIN2, "out");
    set_gpio_direction(SPEEDR, "out");
    set_gpio_direction(SPEEDL, "out");

    set_gpio_direction(LEFT, "in");
    set_gpio_direction(RIGHT, "in");

    set_gpio_direction(TRIG, "out");
    set_gpio_direction(ECHO, "in");

    // PWM ���� (���ļ� 1kHz, ��Ƽ ����Ŭ 50%)
    set_pwm(SPEEDR, 1000, default_speed);
    set_pwm(SPEEDL, 1000, default_speed);
}

void loop()
{
    if (goLeft == 1) {
        turnL();
    }
    else if (goRight == 1) {
        turnR();
    }

    if (goLeft == 0 && goRight == 0) {
        goStr();
    }
}

// GPIO export �Լ�
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

// GPIO ���� ���� �Լ�
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

// GPIO�� �� ���� �Լ�
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

// PWM�� �����ϴ� �Լ�
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
    int period = 1000000000 / frequency; // �ֱ� (������ ����)
    dprintf(fd, "%d", period);  // Period ����
    close(fd);

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", gpio);
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open PWM duty_cycle");
        exit(1);
    }
    int duty = (int)((duty_cycle / 255.0) * period); // ��Ƽ ����Ŭ ����
    dprintf(fd, "%d", duty);  // Duty Cycle ����
    close(fd);

    // Enable PWM
    write(fd, "1", 1);  // Enable PWM
    close(fd);
}

void goStr()
{
    write_gpio(MOTOR_RIN1, 1);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 1);
    write_gpio(MOTOR_LIN2, 0);
}

void goBack()
{
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 1);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 1);
}

void stop()
{
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 0);
}

void turnR()
{
    write_gpio(MOTOR_RIN1, 1);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 1);
}

void turnL()
{
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 1);
    write_gpio(MOTOR_LIN1, 1);
    write_gpio(MOTOR_LIN2, 0);
}

#include <fcntl.h>
#include <string.h>

//���� ����̹�
//������ ����
#define MOTOR_RIN1 17
#define MOTOR_RIN2 27
#define SPEEDR 18
//���� ����
#define MOTOR_LIN1  22 
#define MOTOR_LIN2  23
#define SPEEDL 24

//����Ʈ���̼�
#define LEFT 5
#define RIGHT 6

//��ֹ� ����
#define TRIG 25
#define ECHO 8


//�νķ��� ���� ������
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
void stop();
void turnR();
void turnL();

int goLeft, goRight;

int main() {
    setup();
    while (1) {
        loop();
        sleep(10);
     }

    return 0;
}

void setup()
{
    // GPIO �� export �� ���� ����
    export_gpio(MOTOR_RIN1);
    export_gpio(MOTOR_RIN2);
    export_gpio(MOTOR_LIN1);
    export_gpio(MOTOR_LIN2);
    export_gpio(SPEEDR);
    export_gpio(SPEEDL);
    
    export_gpio(LEFT);
    export_gpio(RIGHT);
    export_gpio(TRIG);
    export_gpio(ECHO);

    set_gpio_direction(MOTOR_RIN1, "out");
    set_gpio_direction(MOTOR_RIN2, "out");
    set_gpio_direction(MOTOR_LIN1, "out");
    set_gpio_direction(MOTOR_LIN2, "out");
    set_gpio_direction(SPEEDR, "out");
    set_gpio_direction(SPEEDL, "out");

    set_gpio_direction(LEFT, "in");
    set_gpio_direction(LEFT, "in");

    set_gpio_direction(TRIG, "out");
    set_gpio_direction(ECHO, "in");

    // PWM ���� (���ļ� 1kHz, ��Ƽ ����Ŭ 50%)
    set_pwm(SPEEDR, 1000, default_speed);
    set_pwm(SPEEDL, 1000, default_speed);
}

void loop()
{
    if (goLeft == 1) {
        turnL();
    }
    else if (goRight == 1) {
        turnR();
    }

    if (goLeft == 0 && goRight == 0) {
        goStr();
    }
}

// GPIO export �Լ�
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

// GPIO ���� ���� �Լ�
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

// GPIO�� �� ���� �Լ�
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

// PWM�� �����ϴ� �Լ�
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
    int period = 1000000000 / frequency; // �ֱ� (������ ����)
    dprintf(fd, "%d", period);  // Period ����
    close(fd);

    snprintf(path, sizeof(path), "/sys/class/pwm/pwmchip0/pwm%d/duty_cycle", gpio);
    fd = open(path, O_WRONLY);
    if (fd == -1) {
        perror("Failed to open PWM duty_cycle");
        exit(1);
    }
    int duty = (int)((duty_cycle / 255.0) * period); // ��Ƽ ����Ŭ ����
    dprintf(fd, "%d", duty);  // Duty Cycle ����
    close(fd);

    // Enable PWM
    write(fd, "1", 1);  // Enable PWM
    close(fd);
}

void goStr()
{
    write_gpio(MOTOR_RIN1, 1);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 1);
    write_gpio(MOTOR_LIN2, 0);
}

void goBack()
{
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 1);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 1);
}

void stop()
{
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 0);
}

void turnR()
{
    write_gpio(MOTOR_RIN1, 1);
    write_gpio(MOTOR_RIN2, 0);
    write_gpio(MOTOR_LIN1, 0);
    write_gpio(MOTOR_LIN2, 1);
}

void turnL()
{
    write_gpio(MOTOR_RIN1, 0);
    write_gpio(MOTOR_RIN2, 1);
    write_gpio(MOTOR_LIN1, 1);
    write_gpio(MOTOR_LIN2, 0);
}
