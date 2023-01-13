//
// Created by anioner on 2023/1/12.
//
#pragma once
unsigned short   *VGA_MEMORY         = (unsigned short *)0xB8000;    // 视频内存地址
unsigned short   VGA_WIDTH           = 80;                           // 屏幕宽度
unsigned short   VGA_HEIGHT          = 25;                           // 屏幕高度
unsigned short   VGA_SCREEN_X        = 0;                            // 屏幕光标X坐标
unsigned short   VGA_SCREEN_Y        = 0;                            // 屏幕光标Y坐标
unsigned short   VGA_CHAR_COLOR      = 0;                            // 屏幕字体与背景颜色

enum VGA_COLOR {
    BLACK   =   0x00/* 黑色 */,   DARK_GRAY       =   0x08/* 暗灰色 */,
    BLUE    =   0x01/* 蓝色 */,   LIGHT_BLUE      =   0x09/* 亮蓝色 */,
    GREEN   =   0x02/* 绿色 */,   LIGHT_GREEN     =   0x0A/* 亮绿色 */,
    CYAN    =   0x03/* 青色 */,   LIGHT_CYAN      =   0x0B/* 亮青色 */,
    RED     =   0x04/* 红色 */,   LIGHT_RED       =   0x0C/* 亮红色 */,
    PURPLE  =   0x05/* 紫色 */,   LIGHT_PURPLE    =   0x0D/* 亮紫色 */,
    BROWN   =   0x06/* 棕色 */,   YELLOW          =   0x0E/* 黄色 */,
    GRAY    =   0x07/* 灰色 */,   WHITE           =   0x0F/* 白色 */,
};


unsigned short vga_make_color(enum VGA_COLOR font_color, enum VGA_COLOR background_color, unsigned short blink){
    // 每个参数按照字节原本所占的比特数去进行与运算，这样可以排除其他比特位的干扰，防止数据错乱
    return (font_color & 0b00001111) | (background_color & 0b00000111) << 4 | (blink & 0b00000001) << 7;
}

unsigned short vga_make_char(unsigned char ch, unsigned short color){
    return ((unsigned short)ch & 0b11111111) | (color & 0b11111111) << 8;
}

// 在特定的位置输出字符
void vga_put_char_at(unsigned char ch, unsigned short x, unsigned short y)
{
    unsigned short vga_char = vga_make_char(ch, VGA_CHAR_COLOR); // 把字符和颜色按照格式合并
    unsigned short index = y * VGA_WIDTH + x;       // 把二维坐标转换为一维坐标
    VGA_MEMORY[index] = vga_char;
}

// 输出单个字符，并且会自动的根据当前的光标位置进行输出。
void vga_put_char(unsigned char ch){
    vga_put_char_at(ch, VGA_SCREEN_X, VGA_SCREEN_Y);
    VGA_SCREEN_X++;
    if(VGA_SCREEN_X == VGA_WIDTH){  // X坐标到底了就换到下一行
        VGA_SCREEN_X = 0;
        VGA_SCREEN_Y++;
        if(VGA_SCREEN_Y == VGA_HEIGHT){ // Y坐标到底了就回到最上面
            VGA_SCREEN_Y = 0;
        }
    }
}

// 输出字符串并返回字符串长度
unsigned short vga_write_string(const char *str){
    unsigned short i = 0;
    while (str[i]){
        vga_put_char(str[i]);
        i++;
    }
    return i + 1;
}

// 设置颜色
void vga_set_color(unsigned short font_color, unsigned short background_color){
    VGA_CHAR_COLOR = vga_make_color(font_color, background_color, 0);
}

// 清空屏幕，原理是往每一个内存地址写入空格，这样就只会留下背景颜色。
void vga_clear_screen(){
    for (unsigned short h = 0; h < VGA_HEIGHT; h++) {
        for (unsigned short w = 0; w < VGA_WIDTH; w++) {
            vga_put_char_at(' ', w, h);
        }
    }
}

// 初始化VGA参数
void vga_init()
{
    vga_set_color(PURPLE, BLACK);
    vga_clear_screen();
}
