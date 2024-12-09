# Design Document

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

## QueryServer

## RconServer
