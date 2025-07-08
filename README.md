# 适用于串口的通信协议驱动

## Requirement
需要先安装 `serial` 库，在ubuntu20中无法直接使用apt安装，故需要从源码安装

```bash
git clone https://github.com/wjwwood/serial.git
cd serial
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install
```

## 通信协议
### 协议帧格式
| 字节索引 | 字段名     | 大小（字节） | 示例值          | 描述说明                                           |
|----------|------------|---------------|-----------------|----------------------------------------------------|
| 0        | 帧头1      | 1             | `0x0F`          | 固定帧头起始标志                                  |
| 1        | 帧头2      | 1             | `0xF0` / `0xFF` | `0xF0` 表示**发送帧**，`0xFF` 表示**接收帧**     |
| 2        | 数据长度   | 1             | `0x04`          | 后续数据字段长度（不包括帧头和校验和）            |
| 3～N     | 数据       | N（可变）     | `0x03, 0xE8 ...`| 以 2 字节为单位的 int16（缩放后 float 数据）    |
| N + 1    | 校验和     | 1             | `0xAB`          | 对前面所有字节求和的低 8 位，用于验证完整性       |

### 数据说明
- 每 2 字节表示一个 `int16_t`，是浮点数 ×1000 后的整数形式。
- 数据数量 = 数据长度 ÷ 2。

### 数据demo
发送发送 float 数组 `{1.0f, -2.0f}`

| 字段名   | 十六进制示例                     | 说明                              |
|----------|----------------------------------|-----------------------------------|
| 帧头1    | `0x0F`                           | 固定值                            |
| 帧头2    | `0xF0`                           | 发送帧                            |
| 长度     | `0x04`                           | 两个 float，共 4 字节（2 x int16） |
| 数据     | `0x03, 0xE8, 0xF8, 0x30`         | `1000` → 1.0，`-2000` → -2.0      |
| 校验和   | `0x??`                           | 对前面所有字节求和取低 8 位       |

## Usage
### 串口及波特率设置
在[config/serial_config.yaml](config/serial_config.yaml)中设置
```
port: "/dev/ttyUSB0"
baudrate: 115200
```

### 函数调用
参照[include/serial_driver/serial_comm.hpp](include/serial_driver/serial_comm.hpp)
```
// init
SerialComm(const std::string& port, unsigned long baudrate);

// send
bool success = serial_comm_ptr->sendFloatArrayCommand(speeds);

// receive
std::vector<float> readFloatArrayResponse();
```

### demo
一个基于ros的示例如[src/serial_main.cpp](src/serial_main.cpp)，为订阅导航模块的 `cmd_vel` 话题并发送对应速度
```
roslaunch serial_driver serial_driver.launch
```