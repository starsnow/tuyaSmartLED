#include <TuyaWifi.h>
#include <SoftwareSerial.h>
#include <FastLED.h>
#include "cube2812.h"

boolean oldState = HIGH;
int mode = 0; // Currently-active animation mode, 0-9

TuyaWifi my_device;

unsigned char led_state = 0;
/* Connect network button pin */

const uint8_t WIFI_RECONNECT_BUTTON_PIN = 3;

//开关(可下发可上报)
//备注:
#define DPID_SWITCH_LED 20
//模式(可下发可上报)
//备注:
#define DPID_WORK_MODE 21
//亮度值(可下发可上报)
//备注:
#define DPID_BRIGHT_VALUE 22
//冷暖值(可下发可上报)
//备注:
#define DPID_TEMP_VALUE 23
//彩光(可下发可上报)
//备注:类型：字符；
//Value: 000011112222；
//0000：H（色度：0-360，0X0000-0X0168）；
//1111：S (饱和：0-1000, 0X0000-0X03E8）；
//2222：V (明度：0-1000，0X0000-0X03E8)
#define DPID_COLOUR_DATA 24
//场景(可下发可上报)
//备注:类型：字符;
//Value: 0011223344445555666677778888;
//00：情景号;
//11：单元切换间隔时间（0-100）;
//22：单元变化时间（0-100）;
//33：单元变化模式（0静态1跳变2渐变）;
//4444：H（色度：0-360，0X0000-0X0168）;
//5555：S (饱和：0-1000, 0X0000-0X03E8);
//6666：V (明度：0-1000，0X0000-0X03E8);
//7777：白光亮度（0-1000）;
//8888：色温值（0-1000）;
//注：数字1-8的标号对应有多少单元就有多少组
#define DPID_SCENE_DATA 25
//倒计时剩余时间(可下发可上报)
//备注:
#define DPID_COUNTDOWN 26
//音乐灯(只下发)
//备注:类型：字符串；
//Value: 011112222333344445555；
//0：   变化方式，0表示直接输出，1表示渐变；
//1111：H（色度：0-360，0X0000-0X0168）；
//2222：S (饱和：0-1000, 0X0000-0X03E8）；
//3333：V (明度：0-1000，0X0000-0X03E8）；
//4444：白光亮度（0-1000）；
//5555：色温值（0-1000）
#define DPID_MUSIC_DATA 27
//调节(只下发)
//备注:类型：字符串 ;
//Value: 011112222333344445555  ;
//0：   变化方式，0表示直接输出，1表示渐变;
//1111：H（色度：0-360，0X0000-0X0168）;
//2222：S (饱和：0-1000, 0X0000-0X03E8);
//3333：V (明度：0-1000，0X0000-0X03E8);
//4444：白光亮度（0-1000）;
//5555：色温值（0-1000）
#define DPID_CONTROL_DATA 28
//入睡(可下发可上报)
//备注:灯光按设定的时间淡出直至熄灭
#define DPID_SLEEP_MODE 31
//唤醒(可下发可上报)
//备注:灯光按设定的时间逐渐淡入直至设定的亮度
#define DPID_WAKEUP_MODE 32
//断电记忆(可下发可上报)
//备注:通电后，灯亮起的状态
#define DPID_POWER_MEMORY 33
//勿扰模式(可下发可上报)
//备注:适用经常停电区域，开启通电勿扰，通过APP关灯需连续两次上电才会亮灯
//Value：ABCCDDEEFFGG
//A：版本，初始版本0x00；
//B：模式，0x00初始默认值、0x01恢复记忆值、0x02用户定制；
//CC：色相 H，0~360；
//DD：饱和度 S，0~1000；
//EE：明度 V，0~1000；
//FF：亮度，0~1000；
//GG：色温，0~1000；
#define DPID_DO_NOT_DISTURB 34
//麦克风音乐律动(可下发可上报)
//备注:类型：  字符串
//Value：  AABBCCDDEEFFGGGGHHHHIIIIJJJJKKKKLLLLMMMMNNNN
//AA  版本
//BB  0-关闭，1-打开
//CC  模式编号，自定义从201开始
//DD  变换方式：0 - 呼吸模式，1 -跳变模式 ， 2 - 经典模式
//EE  变化速度
//FF  灵敏度
//GGGG  颜色1-色相饱和度
//HHHH  颜色2-色相饱和度
//......
//NNNN  颜色8-色相饱和度
#define DPID_MIC_MUSIC_DATA 42
//炫彩情景(可下发可上报)
//备注:专门用于幻彩灯带场景
//Value：ABCDEFGHIJJKLLM...
//A：版本号；
//B：情景模式编号；
//C：变化方式（0-静态、1-渐变、2跳变、3呼吸、4-闪烁、10-流水、11-彩虹）
//D：单元切换间隔时间（0-100）;
//E：单元变化时间（0-100）；
//FGH：设置项；
//I：亮度（亮度V：0~100）；
//JJ：颜色1（色度H：0-360）；
//K：饱和度1 (饱和度S：0-100)；
//LL：颜色2（色度H：0-360）；
//M：饱和度2（饱和度S：0~100）；
//注：有多少个颜色单元就有多少组，最多支持20组；
//每个字母代表一个字节
#define DPID_DREAMLIGHT_SCENE_MODE 51
//炫彩本地音乐律动(可下发可上报)
//备注:专门用于幻彩灯带本地音乐
//Value：ABCDEFGHIJKKLMMN...
//A：版本号；
//B：本地麦克风开关（0-关、1-开）；
//C：音乐模式编号；
//D：变化方式；
//E：变化速度（1-100）;
//F：灵敏度(1-100)；
//GHI：设置项；
//J：亮度（亮度V：0~100）；
//KK：颜色1（色度H：0-360）；
//L：饱和度1 (饱和度S：0-100)；
//MM：颜色2（色度H：0-360）；
//N：饱和度2（饱和度S：0~100）；
//注：有多少个颜色单元就有多少组，最多支持8组；
//每个字母代表一个字节
#define DPID_DREAMLIGHTMIC_MUSIC_DATA 52
//点数/长度设置(可下发可上报)
//备注:幻彩灯带裁剪之后重新设置长度
#define DPID_LIGHTPIXEL_NUMBER_SET 53

///* Current device DP values */
unsigned char dp_bool_value = 0;
long dp_value_value = 0;
unsigned char dp_enum_value = 0;
unsigned char dp_string_value[21] = {"0"};
uint16_t Hue = 0; //HSV
uint8_t Sat = 0;
uint8_t Val = 0;
uint8_t scene_mode = 0;
unsigned char hex[10] = {"0"};
//unsigned char dp_raw_value[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
//int dp_fault_value = 0x01;

/* Stores all DPs and their types. PS: array[][0]:dpid, array[][1]:dp type. 
 *                                     dp type(TuyaDefs.h) : DP_TYPE_RAW, DP_TYPE_BOOL, DP_TYPE_VALUE, DP_TYPE_STRING, DP_TYPE_ENUM, DP_TYPE_BITMAP
*/
unsigned char dp_array[][2] = {
    {DPID_SWITCH_LED, DP_TYPE_BOOL},
    {DPID_WORK_MODE, DP_TYPE_ENUM},
    {DPID_BRIGHT_VALUE, DP_TYPE_VALUE},
    {DPID_TEMP_VALUE, DP_TYPE_VALUE},
    {DPID_COLOUR_DATA, DP_TYPE_STRING},
    {DPID_SCENE_DATA, DP_TYPE_STRING},
    {DPID_COUNTDOWN, DP_TYPE_VALUE},
    {DPID_MUSIC_DATA, DP_TYPE_STRING},
    {DPID_CONTROL_DATA, DP_TYPE_STRING},
    {DPID_SLEEP_MODE, DP_TYPE_RAW},
    {DPID_WAKEUP_MODE, DP_TYPE_RAW},
    {DPID_POWER_MEMORY, DP_TYPE_RAW},
    {DPID_DO_NOT_DISTURB, DP_TYPE_BOOL},
    {DPID_MIC_MUSIC_DATA, DP_TYPE_STRING},
    {DPID_DREAMLIGHT_SCENE_MODE, DP_TYPE_RAW},
    {DPID_DREAMLIGHTMIC_MUSIC_DATA, DP_TYPE_RAW},
    {DPID_LIGHTPIXEL_NUMBER_SET, DP_TYPE_VALUE},
};

unsigned char pid[] = {"vhj6p7rzvoemihhx"}; //*********处替换成涂鸦IoT平台自己创建的产品的PID
unsigned char mcu_ver[] = {"1.0.0"};

void setup()
{
    pinMode(WIFI_RECONNECT_BUTTON_PIN, INPUT_PULLUP);

    Serial.begin(9600);
    //Initialize led port, turn off led.
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    //Initialize networking keys.
    //incoming all DPs and their types array, DP numbers
    //Enter the PID and MCU software version
    my_device.init(pid, mcu_ver);
    my_device.set_dp_cmd_total(dp_array, 17);
    //register DP download processing callback function
    my_device.dp_process_func_register(dp_process);
    //register upload all DP callback function
    my_device.dp_update_all_func_register(dp_update_all);

    last_time = millis();

    initCube2812();
}

void loop()
{
    //Enter the connection network mode when Pin7 is pressed.
    if (digitalRead(WIFI_RECONNECT_BUTTON_PIN) == LOW)
    {
        delay(80);
        if (digitalRead(WIFI_RECONNECT_BUTTON_PIN) == LOW)
        {
            my_device.mcu_set_wifi_mode(SMART_CONFIG);
        }
    }
    my_device.uart_service();

    /* LED blinks when network is being connected */
    if ((my_device.mcu_get_wifi_work_state() != WIFI_LOW_POWER) && (my_device.mcu_get_wifi_work_state() != WIFI_CONN_CLOUD) && (my_device.mcu_get_wifi_work_state() != WIFI_SATE_UNKNOW))
    {
        if (millis() - last_time >= 500)
        {
            last_time = millis();

            if (led_state == LOW)
            {
                led_state = HIGH;
            }
            else
            {
                led_state = LOW;
            }
            digitalWrite(LED_BUILTIN, led_state);
        }
    }

    updateCub2812();
}

/**
 * @description: DP download callback function.
 * @param {unsigned char} dpid
 * @param {const unsigned char} value
 * @param {unsigned short} length
 * @return {unsigned char}
 */
unsigned char dp_process(unsigned char dpid, const unsigned char value[], unsigned short length)
{
    switch (dpid)
    {
    case DPID_SWITCH_LED:
        dp_bool_value = my_device.mcu_get_dp_download_data(dpid, value, length); /* Get the value of the down DP command */
        if (dp_bool_value)
        {
            //Turn on
            renderFunction = lastRenderFunction;
            FastLED.clear();
        }
        else
        {
            //Turn off
            lastRenderFunction = renderFunction;
            renderFunction = 0;
            FastLED.showColor(CRGB::Black);
        }

        //Status changes should be reported.
        my_device.mcu_dp_update(dpid, value, length);
        break;

    case DPID_WORK_MODE:
        dp_enum_value = my_device.mcu_get_dp_download_data(dpid, value, length); /* Get the value of the down DP command */
        switch (dp_enum_value)
        {
        case 0: // white mode
            break;
        case 1: // colour mode

            break;
        case 2: // scene mode

            break;
        case 3: // music mode

            break;
        }
        //Status changes should be reported.
        my_device.mcu_dp_update(dpid, value, length);
        break;

    case DPID_COUNTDOWN: //倒计时
        my_device.mcu_dp_update(dpid, value, length);
        break;

    case DPID_MUSIC_DATA: //音乐律动
        my_device.mcu_dp_update(dpid, value, length);
        break;

    case DPID_SCENE_DATA:
    blinkLED(1);
       my_device.mcu_dp_update(DPID_DREAMLIGHT_SCENE_MODE, value, length);

        scene_mode = value[1];

        switch (scene_mode)
        {
        case 0:
            renderFunction = renderRainbow;
            break;
        case 1:
            renderFunction = renderHackMatrix;
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            break;
        case 8:
            break;
        }
        break;

    case DPID_DREAMLIGHT_SCENE_MODE: //炫彩情景
        blinkLED(2);
        //delay(1000);
        my_device.mcu_dp_update(DPID_DREAMLIGHT_SCENE_MODE, value, length);

        scene_mode = value[1];
blinkLED(scene_mode);
        switch (scene_mode)
        {
        case 0:
            renderFunction = renderRainbow;
            break;
        case 1:
            renderFunction = renderHackMatrix;
            break;
        case 2:
            renderFunction = renderRainbow;
            break;
        case 3:
            renderFunction = renderRainbow;
            break;
        case 4:
            renderFunction = renderRainbow;
            break;
        case 5:
            renderFunction = renderRainbow;
            break;
        case 6:
            renderFunction = renderRainbow;
            break;
        case 7:
            break;
        case 8:
            break;
        }

        break;

    case DPID_LIGHTPIXEL_NUMBER_SET: //长度设置
        my_device.mcu_dp_update(dpid, value, length);
        break;
    default:
        break;
    }
    return SUCCESS;
}

/**
 * @description: Upload all DP status of the current device.
 * @param {*}
 * @return {*}
 */
void dp_update_all(void)
{
    my_device.mcu_dp_update(DPID_SWITCH_LED, led_state, 1);
}