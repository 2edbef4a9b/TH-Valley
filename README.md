# TH-Valley

设计一个农场生活模拟游戏, 玩家将管理自己的农场, 种植作物, 养殖动物, 与镇上的居民互动, 参与节日活动, 并探索周围的自然环境。游戏应包含丰富的季节变化、社区交互和个人角色发展.

## 总体设计

### 服务端/客户端架构

游戏将采用客户端/服务端架构, 将前后端分离, 分离职能, 同时易于实现多人游戏.

#### 服务端

服务端负责处理游戏逻辑, 执行 `background tasks`, 并定时存储游戏状态. 分为以下几个模块:

1. [`UniverseServer`](#universeserver)
   - 游戏世界的主要逻辑
   - 负责管理游戏世界的状态
   - 处理客户端连接和请求
2. [`QueryServer`](#queryserver)
   - 处理客户端的查询请求
   - 提供服务器状态的快照
   - 提供游戏世界的只读数据
3. [`RconServer`](#rconserver)
   - 提供远程控制服务
   - 用于管理服务器状态
   - 保存游戏状态
   - 管理玩家账户

三个客户端的设计的目的是为了分离职能, 使得服务端的逻辑更加清晰, 便于维护和扩展, 优先开发 `UniverseServer`, 后续再开发 `QueryServer` 和 `RconServer`.

#### 客户端

客户端 [`ClientApplication`](#clientapplication) 负责处理用户输入, 渲染游戏画面, 并与服务端通信.
作为程序的入口点, 控制游戏的运行, 是一个 `cocos2d::Application` 的实例.

主要功能包括:

1. 启动和关闭
   - `StartUp` : 初始化客户端应用程序, 包括加载配置和资源.
   - `ShutDown` : 关闭客户端应用程序, 释放资源.
2. 初始化:
   - `InitApplicaiton` : 初始化游戏应用程序控制器, 音频, 输入等.
   - `InitRenderer` : 初始化游戏渲染器和界面.
3. 更新和渲染:
   - `Update` : 更新客户端的状态和逻辑, 涉及到与服务端的通信.
   - `Render` : 渲染客户端界面和游戏世界.
4. 输入处理:
   - `ProcessInput` : 处理用户输入事件.
5. 状态管理:
   - `ChangeState` : 切换游戏状态, 涉及到与服务端的通信, 以及更新服务端状态.
6. 错误处理:
   - `HandleError` : 处理并显示错误信息.

### 游戏运行流程

游戏运行由客户端 `ClientApplication` 控制, 包括以下几个阶段:

``` Mermaid
graph TD
    StartUp --> InitApplication
    InitApplication --> InitRenderer
    InitRenderer --> ProcessInput
    ProcessInput --> Update
    Update --> Render
    Render -->|Quit?| ShutDown
    Render --> ProcessInput
```

## UniverseServer

`UniverseServer` 以周期性的方式更新游戏世界的状态, 每秒 60 个周期, 一个周期为一个 `gametick`. 每个 `gametick` 包括以下几个步骤:

1. WTU (World Time Update) 时间更新
   - 同步客户端的时间 (总 `gametick` 数)
   - Note: 首先更新时间, 以确保客户端的时间的一致性
2. CT (Chunk Tick) 区块刻
   - 更新游戏世界中的区块状态, 包括加载和卸载区块, 更新区块中的方块状态等
3. ST (Scheduled Tick) 计划刻
   - 处理带有延迟的任务, 创建 NTE (Next Tick Entry) 列表, 用于存储计划在后续 `gametick` 中执行的任务
4. RT (Random Tick) 随机刻
   - 随机更新游戏世界的状态, 包括天气, 作物状态变化, 生物生成等
5. NPCU (Non-Player Character Update) 非玩家角色更新
   - 更新游戏世界中的 NPC 的状态, 交互, 行为等
6. BE (Block Event) 方块事件
   - 处理方块事件, 包括方块的交互, 状态变化等
7. EU (Entity Update) 实体更新
   - 更新游戏世界中除玩家外的实体的状态, 包括各类生物, 掉落物品等
8. NU (Network Update) 网络更新, 即处理客户端的请求, 并更新客户端的状态
   - 处理客户端的请求, 包括玩家的移动, 交互, 以及其他请求
   - 响应 `RconServer` 的请求
   - 更新客户端的状态
   - Note: 最后处理客户端的请求, 以确保客户端的状态是最新的
9. AS (Auto Save) 自动保存
   - 每 18,000 个 `gametick` (300 秒) 自动保存游戏状态
10. 更新 `QueryServer` 的状态
11. 等待下一个 `gametick`

### 存储在 UniverseServer 中的数据

1. 加载的 [`Scene`](#scene) 数据
2. 每个 `Scene` 中的非玩家实体数据 [`NonPlayerEntityData`](#nonplayerentitydata)
3. 每个玩家的数据 [`AvatarData`](#avatardata)
4. 游戏的全局状态 [`GameState`](#gamestate)
5. 服务器信息 [`ServerInfo`](#serverinfo)
6. 服务器运行日志 [`ServerLog`](#serverlog)

#### Scene

#### NonPlayerEntityData

#### AvatarData

#### GameState

#### ServerInfo

#### ServerLog

### UniverseServer 的运行

## QueryServer

## RconServer

## ClientApplication

客户端应用程序 `ClientApplication` 以与服务端相同的周期读取用户输入, 服务端未运行的情况下, 提供 GUI 界面, 服务器运行时, 通过网络连接到服务端, 与服务端通信.

### 存储在 ClientApplication 中的数据

1. 当前用户的档案数据 [`UserProfile`](#userprofile)
   - 用户的名称 `user_name_`, 用户在游戏中的名字
   - 用户的 UUID `user_uuid_`, 用户的唯一标识符, 用于确定用户的身份
   - Note: 使用 JSON 格式存储
2. 当前用户的配置数据 [`UserConfig`](#userconfig)
   - 用户的设置 `user_settings_`, 用户的游戏设置, 包括音量, 分辨率等
   - 用户的键位设置 `user_keybindings_`, 用户的键位设置, 包括移动, 交互等
   - Note: 使用 JSON 格式存储
3. 客户端当前的状态 [`ClientState`](#clientstate)
   - 客户端的状态 `client_state_`, 可能的状态包括 `TitleScreen`, `SinglePlayer`, `MultiPlayer`, `Settings`, `Loading`, `Quit`, `StartUp`, `ShutDown`.
4. 游戏玩家 [`Avatar`](#avatar)
   - `Avatar` 仅保存必要的数据, 包括玩家的位置, 朝向, 行走状态等, 其他数据保存在服务端
5. 连接的服务器 [`ConnectedServer`](#connectedserver)
   - 服务器的 IP 地址 `server_ip_`, 服务器的 IP 地址
   - 服务器的端口 `server_port_`, 服务器的端口
   - 服务器的状态 `server_status_`, 服务器的状态, 包括 `Offline`, `Online`, `Connecting`, `Connected`, `Disconnecting`

#### UserProfile

#### UserConfig

#### ClientState

#### Avatar

#### ConnectedServer

### ClientApplication 与 UniverseServer 的通信

#### 通信协议

#### 通信格式

使用 JSON 格式进行通信, 以便于解析和处理, 包含两个 field, `type` 和 `data`, `type` 用于标识消息的类型, `data` 用于存储消息的数据.

``` JSON
{
    "type": "message_type",
    "data": {
        "field1": "value1",
        "field2": "value2",
        ...
    }
}
```

客户端 -> 服务端的消息

1. `ConnectRequest` 连接消息
   - `type`: `connect_request`
   - `data`: `{"user_name": "user_name", "user_uuid": "user_uuid"}`
   - Note: 用户连接到服务器时发送其用户名和 UUID, 此后不再发送用户名和 UUID, 由服务端创建用于与此用户通信的 `Session`
2. `DisconnectRequest` 断开连接消息
   - `type`: `disconnect_request`
   - `data`: `{}`
3. `ActionRequest` 动作请求消息
   - `type`: `action_request`
   - `data`: `{"action": "action_type", "target_position": "target_position"}`
   - Note: 客户端的动作请求, 包括玩家的移动, 攻击, 交互等, 攻击等动作也涉及到玩家的位置移动
4. `Error` 错误消息
   - `type`: `error`
   - `data`: `{"error_code": "error_code", "error_message": "error_message"}`
   - Note: 客户端的错误消息, 包括错误代码和错误信息
5. `HeartBeat` 心跳消息
   - `type`: `heartbeat`
   - `data`: `{}`
   - Note: 客户端的心跳消息, 用于保持连接, 每 1800 个 `gametick` 发送一次

服务端响应客户端的消息

1. `ConnectResponse` 连接响应消息
   - `type`: `connect_response`
   - `data`: `{"status": "status", "message": "message"}`
   - Note: 服务端的连接响应, 包括连接状态和消息
2. `DisconnectResponse` 断开连接响应消息
   - `type`: `disconnect_response`
   - `data`: `{"status": "status", "message": "message"}`
   - Note: 服务端的断开连接响应, 包括连接状态和消息
3. `ActionResponse` 动作响应消息
   - `type`: `action_response`
   - `data`: `{"action": "action_type", "revised_position": "revised_position"}`
   - Note: 服务端的动作响应, 对玩家的目标位置进行修正, 以确保玩家的位置是合法的
4. `Error` 错误消息 (不需要响应, 直接记录到日志, 并做出相应的处理)
5. `HeartBeat` 心跳消息
   - `type`: `heartbeat`
   - `data`: `{"status": "status", "gametick": "gametick"}`
   - Note: 服务端的心跳消息, 用于保持连接, 返回服务端的状态和当前的 `gametick` 来保持客户端的时间同步

服务端 -> 客户端的消息

1. `AvatarUpdate` 更新消息
   - `type`: `update`
   - `data`: `{"avatar_data": "avatar_data"}`
   - Note: 服务端更新玩家的数据, 针对特定玩家发送, 更新玩家的 `health`, `hunger`, `stamina` 等数据
2. `WorldUpdate` 更新消息
   - `type`: `update`
   - `data`: `{"world_data": "world_data"}`
   - Note: 服务端更新游戏世界的数据, 针对所有玩家发送, 更新游戏世界的状态, 包括天气, 时间等
3. `EntityUpdate` 更新消息
   - `type`: `update`
   - `data`: `{"entity_data": "entity_data"}`
   - Note: 服务端更新实体的数据, 针对所有玩家发送, 更新实体的状态, 包括生物, NPC等.

消息的处理

1. 消息队列: 客户端和服务端均使用消息队列, 用于存储消息, 并在每个 `gametick` 处理消息


## Key
- read the input of player and do the related reaction

## how to make it

1.ask player to input player imformation and reset the recent imfromation

2.use lamada expression to finish the change

3.CCLOG the massage when one operate is over

## Agriculture
---
Brief: The basic class supporting agriculture in game.

### Member

- CurrentYear
- CurrentMonth
- CurrentDay
- CurrentHour
- CurrentSeason
- CurrentWeather
- Temperature

## CropProduction
---
Brief: The derived class from Agriculture

### Member

- PlayerCropLevel
- TotalCrops
- Crops(vector)

## Crops
---
Brief: The derived class from Agriculture

### Member

- CurrentGrowthStage
- MaxGrowthStage
- GrowthDuration(vector)
- GrowthSituation
- WaterRequirement
- FertilizerRequirement
- SoilRequirement
- Position
- PlayerPosition

### Member Function

#### showCropInfo
Brief: A virtual function. Show the information of crops, and offer the operation for crops like watering and fertilizing

## Strawberry
---
Brief: An example of the specific crops, the derived class from Crops.

- spritePng
