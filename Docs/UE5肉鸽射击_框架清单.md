# UE5 肉鸽射击 — 框架清单

> **引擎版本**：UE 5.7.3 / Windows 11
> **项目类型**：蓝图为主，GAS 相关用 C++
> **开发者水平**：有 C++ 基础（面向对象 / STL / 常用特性）；蓝图已能独立搭系统
>
> **当前目标（只做这两件）**
> 1. **彻底改成 GAS** —— 伤害、属性、技能全部走 GameplayAbilitySystem
> 2. **完全整理和清理** —— 命名、目录、死资产、文档
>
> **暂缓**：具体玩法设计、肉鸽循环、美术替换
>
> **长期目标**：5 个月内做出可写进简历的 Gameplay + 性能优化方向作品

---

## 使用说明

- 状态标记：`[x]` 完成 / `[~]` 进行中 / `[ ]` 未开始
- **顺序是按依赖关系排的，不要跳项**
- 第 3 个月底必须打完「阶段一 + 阶段二」，否则**砍功能，不砍美术时间**
- 每项都写了「验收标准」，达不到就不算完成
- **当前只推进标了 P0 的部分**，其余先冻住

---

## 进度总览

| 阶段 | 内容 | 状态 | 当前优先级 |
|---|---|---|---|
| 阶段一 | 核心战斗地基 | **4/7 完成** | **P0（推进中）** |
| 阶段二 | 肉鸽循环 | 未开始 | 暂缓 |
| 阶段三 | 流程与 UI | 未开始 | 暂缓 |
| 跨阶段 | 手感 | 第一批完成 | 待 6 项完成后继续 |
| 阶段四 | 美术与音效替换 | 未开始 | 暂缓 |
| 阶段五 | 性能优化实战 | 未开始 | P1（阶段一收尾后开始） |

---

# 阶段一：核心战斗地基

> 目标：把「射击游戏」做扎实。这一阶段的产出应该是一个手感能玩的射击游戏。

## [x] 1. 角色移动 + 相机 + 射击

**已完成**：基础移动逻辑、射击逻辑、练枪关卡。

**遗留检查**：
- [x] 移动参数是否外置成变量？（速度、跳跃高度）
- [x] 射击逻辑是否放在组件里，还是全塞在角色蓝图里？
- [x] 输入用的是 Enhanced Input 吗？（5.7 默认是）

**后续追加**（2026-09 手感批次）：
- [x] 冲刺（Shift）：速度 + FOV
- [x] 滑铲（C / Ctrl）：降摩擦 + 相机下沉 + 速度阈值结束
- [x] 速度驱动 FOV、镜头晃动（bob）、空中关闭晃动
- [x] 相机结构修正：**胶囊只跟 Yaw / 相机接管 Pitch / 手臂挂相机下**

---

## [x] 2. 武器系统 + 数据驱动

**为什么排第二**：这是整个项目的地基。伤害、词条、升级、掉落全部要挂在武器数据上。

**要做的事**：
- [x] 定义武器数据结构：射速、单发伤害、弹匣容量、换弹时间、射程、散射、后坐力、子弹类型
- [x] 用 **DataAsset** 存这些数据
- [x] 把射击逻辑抽成组件
- [x] 支持至少 3 把武器（手枪 / 步枪 / 霰弹枪）
- [x] 支持切枪
- [x] 弹匣 + 换弹逻辑
- [x] 开火时通知 UI（弹药变化）

**实际做法（与原始计划的差异）**：
- 武器是**独立的 Actor `BP_GunBase`**，不是挂在角色上的组件。已达成"逻辑不在角色里、敌人可复用"的目标
- 字段清单：`FireInterval` `Damage` `Range` `Spread` `Recoil` `MagazineSize` `ReloadTime` `PelletsPerShot` `bAutomatic` `MaxReserveAmmo` `CritChance` `CritMultiplier` `WeaponMesh`
- 切枪用**开局全部生成 + 可见性切换**，不 `SpawnActor`

**验收标准**：
> 不改任何蓝图节点，只在编辑器里新建一份武器数据资产，就能做出一把新武器。 ✅ 已通过

---

## [x] 3. 伤害系统

**要做的事**：
- [x] 统一伤害入口
- [x] 支持暴击（暴击率 / 暴击倍率）
- [x] 支持伤害倍率修正（词条加成接口）
- [x] 受击反馈：击退、受击音效
- [x] 死亡处理统一：广播死亡事件
- [x] 「可受伤」契约

**实际做法（2026-10 已迁 GAS）**：

| 系统 | 现状 |
|---|---|
| 属性 | `MyAttributeSet`（C++，`ATTRIBUTE_ACCESSORS_BASIC`） |
| 伤害 | `GE_Damage`（Instant + `SetByCaller(Data.Damage)`） |
| 死亡 | `Wait for Attribute Changed` → 原 GameMode 流程 |
| `BP_HealthComponent` | 已从角色删除 |

- 伤害计算：`CalculateFinalDamage` 纯函数（攻击方算）→ 结果喂 `Set By Caller` → GE 施加
- 受击闪烁延后到阶段四（需要带参数的材质）

**验收标准**：
> 敌人受伤、玩家受伤、可破坏物受伤，走的是同一套逻辑。 ✅ 已通过

---

## [x] 4. 敌人 AI

**已完成**：`NavMeshBoundsVolume` 已放进关卡；两种敌人（近战 / 远程）已跑通。

**前置清理**：
- [x] 删掉 `BP_EnemyBase` 里旧的 `Set Timer by Event`（0.2 秒）+ `AI Move To`，避免与行为树抢控制权

**命名规范**：`AIC_` 控制器 / `BB_` 黑板 / `BT_` 行为树 / `BTT_` 任务 / `BTD_` 装饰器 / `BTS_` 服务

**要做的事**：
- [x] 至少 2 种敌人类型，行为有差异（近战冲锋 / 远程射击）
- [x] 用 `AIController + Behavior Tree + Blackboard`
- [x] 发现玩家 → 追击 → 攻击 → 失去目标
- [x] 攻击有前摇、间隔
- [x] 敌人之间不互相严重重叠
- [x] 敌人参数外置

**当前结构**：

```
AIC_Enemy + BB_Enemy
├─ BT_Enemy_Melee   → BTT_MeleeAttack
└─ BT_Enemy_Ranged  → BTT_RangedAttack
   （共用 BTS_UpdateCombatRange，每 0.3 秒刷新目标 / 距离）

BP_EnemyBase → BP_Enemy_Melee / BP_Enemy_Ranged
BP_EnemyData → DA_Enemy_Melee / DA_Enemy_Ranged
SA_EnemySpawn3D（刷怪点）
```

**敌人攻击已走 GAS**：行为树 Task 内施加 `GE_Damage`，不再用 `Apply Damage`。

**BP_EnemyData 字段**：
`MaxHealth` `MoveSpeed` `ChaseSpeed` `AttackDamage` `AttackRange` `AttackCooldown` `SightRadius` `LoseSightTime`

**关键概念**：
- `AIController` —— AI 的大脑，和角色是分开的两个东西
- `Behavior Tree` —— 每帧从根重新评估的决策树；不存数据
- `Blackboard` —— 行为树的共享数据板
- `NavMesh` —— AI 移动的前提。**记得在关卡里放 NavMeshBoundsVolume**

**验收标准**：
> 两种敌人在同一场景里行为明显不同；敌人不会卡在墙上原地抽搐；打死敌人后原有的伤害/死亡逻辑仍正常。 ✅ 已通过

**已知坑（本项目已踩）**：

| 坑 | 现象 | 解法 |
|---|---|---|
| 忘记 NavMesh | AI 完全不动，不报错 | 按 `P` 确认绿色覆盖 |
| **`BlackboardKeySelector` 变量设不了** | `Set Blackboard Value` **静默失败** | **改用 `Get Blackboard` + `Set Value as X`（键名是普通字符串）** |
| `Move To` 的黑板键没设 | 节点显示 `move target: None / (invalid)`，任务立即失败 | 选中节点在 Details 里设 |
| 节点左右顺序错 | 序列行为完全不对 | 行为树从左到右执行 |
| 行为树直接改子对象属性 | 耦合 | 在角色蓝图里开函数，行为树调它 |
| **敌人被自己的碰撞挖空脚下导航网格** | 关卡里摆的敌人不动，动态生成能动 | 敌人组件取消 `Can Ever Affect Navigation`，再重建路径 |

**行为树调试方法（必用）**：
> 跑起来，看三样：**行为树面板**（哪个节点高亮/失败）、**黑板面板**（各键实时值）、**节点属性文字**（`move target: xxx`）。连线看不出的问题全在这里。

---

## [~] 5. 生成系统（波次 + 难度曲线）　**← 暂停**

**暂停原因**：先把 GAS 和项目结构理顺，再回来做玩法。

**要做的事**：
- [ ] 波次管理器：按波生成，波与波之间有间隔
- [ ] 难度曲线：每波敌人数量/血量/类型按规则递增
- [ ] 生成本身要**不卡顿**
- [ ] 生成位置：玩家周边环形，不在视野内贴脸生成
- [ ] 场上敌人上限控制

**实施要点**：
```
① 波次管理器挂在 GameMode 或独立 Actor 上
② 每波：按权重抽敌人类型 → 在玩家周围环形取点（用导航可达性验证）
③ 生成间隔用定时器，不要一帧内生成完
④ 敌人数量/血量按波数查曲线（数据资产或曲线资产）
⑤ 与已有钩子对接：敌人 OnDeath → 计数器减一 → 本波清空则开下一波
```

**关键概念**：
- **对象池（Object Pooling）** —— 预先生成一批敌人反复复用，避免运行时 `SpawnActor` 卡顿
- `Weighted Random` —— 按权重随机，比纯随机有节奏感

**验收标准**：
> 连续打 10 波不掉帧；敌人从 20 涨到 200 时，能说出帧率变化了多少。

**常见坑**：
- 每波 `SpawnActor` 几十个 → 明显卡顿
- 难度只有数量递增 → 玩到后面只是变卡，不是变难

---

## [ ] 6. GAS 迁移收尾　**← P0，进行中**

> 迁了约 70%。剩下的是「让数据真正驱动属性」和「把命名/目录理顺」。

### [ ] 6.1 `GE_InitStats`：把 DataAsset 的值灌进属性

**现状问题**：`MyAttributeSet` 构造函数里 `InitHealth(100)` 是写死的 → **所有敌人都是 100 血**，`DA_Enemy_*.MaxHealth` 没生效。

**做法**：

```
GE_InitStats
    Duration Policy = Instant
    Modifiers:
        Health      Op = Override   Magnitude = SetByCaller(Data.MaxHealth)
        MaxHealth   Op = Override   Magnitude = SetByCaller(Data.MaxHealth)
```

敌人 `BeginPlay`（顺序不能乱）：

```
① Init Stats( MyAttributeSet )                       ← 先注册属性
② Make Effect Context
   → Make Outgoing Spec( GE_InitStats, 1.0, Context )
   → Assign Tag Set By Caller Magnitude( Data.MaxHealth, EnemyData.MaxHealth )
   → Apply Gameplay Effect Spec To Self
③ Wait for Attribute Changed( Health )               ← 必须在 ②之后
```

**验收**：150 血的敌人和 80 血的敌人满血值不同；改 DataAsset 数值后重启 PIE 立刻生效，**不改任何蓝图**。

### [ ] 6.2 玩家的初始属性也走同一套

玩家的 `MaxHealth` 现在也是构造函数写死的。肉鸽里生命上限要能升级，所以迟早要挪。

### [ ] 6.3 决定 `AttackDamage` 要不要进属性

| 选择 | 后果 |
|---|---|
| **留 DataAsset** | 行为树 Task 读出来塞 `SetByCaller`，已经够数据驱动 |
| **进属性** | 能被 "狂暴：攻击力 +50%" 这类 Effect 改 |

**判断法则**：会被 GameplayEffect 改的 → 属性；只是配置的 → DataAsset。

### [ ] 6.4 敌人技能是否数据驱动（以后再做）

```
BP_EnemyData 加：Abilities : TArray<TSubclassOf<UGameplayAbility>>
敌人 BeginPlay：遍历 → Give Ability
```

### [ ] 6.5 换弹蒙太奇（可选，需要动画管线）

现在是 `AbilityTask_WaitDelay`。要做动画，得换成 `AbilityTask_PlayMontageAndWait`，前置是：手臂骨骼网格 + AnimBP + 蒙太奇资产。

---

## [ ] 7. 项目整理与清理　**← P0，未开始**

> 把过去两个月欠的账一次还掉。**改名在编辑器内做会自动修引用，越早越便宜。**

### [ ] 7.1 命名欠账

| 现在 | 改成 |
|---|---|
| `MyAttributeSet` | `RushBattleAttributeSet` |
| `GE_FireCooldoown` | `GE_FireCooldown` |
| 文件夹 `GamepalyEffect` | `GameplayEffect` |
| `UI_MainUi` | `WBP_MainUI` |
| `IA_Number_1` / `IA_Number_2` | `IA_Weapon1` / `IA_Weapon2` |
| `IA_Key_SwitchWeapon` | `IA_NextWeapon` |
| `SA_EnemySpawn3D` | `BP_EnemySpawnPoint` |
| 文件夹 `BluePointInterface` | `BlueprintInterface` |
| 文件夹 `GameplayAbilitySystem` | `GAS`（可选） |

### [ ] 7.2 目录结构

**目标**：

```
Content/
├─ Code/                  游戏逻辑
│   ├─ Player/
│   ├─ Weapon/
│   ├─ Enemy/
│   ├─ GAS/               GA / GE
│   ├─ UI/
│   └─ Input/
├─ Assets/                第三方资源（gitignore）
├─ Maps/
└─ Test/                  测试资产，可整体删除
```

**要处理的**：
- [ ] `Enemy/Enemy/` 多出来的一层
- [ ] `Player/Temp_Anim/` 临时动画目录 —— 整理成 `Weapon/Arms/` 或删
- [ ] `Weapon/Gun/Data/` 与 `Weapon/Data/` 是否合并
- [ ] `TestMap` 移入 `Maps/`

### [ ] 7.3 死资产清理

| 资产 | 处理 |
|---|---|
| `BP_HealthComponent` | **删除**（已不用） |
| `GA_Test` | 删除或移入 `Test/` |
| `BP_EnemyTest` / `DA_EnemyTest` | 移入 `Test/` |
| `BP_Enemy_1`（旧版） | 检查引用后删 |
| 空文件夹 / Redirector | `右键 → Fix Up Redirectors in Folder` |

**检查方法**：右键资产 → `Reference Viewer`，确认没被引用再删。

### [ ] 7.4 蓝图内部清理

- [ ] 删掉所有临时 `Print String`
- [ ] `BP_GunBase` 里残留的 `Start Fire` / `Stop Fire`（如果已被 GA 取代）
- [ ] `Apply Point Damage` / `Apply Damage` 残留节点
- [ ] `My Blueprint` 里删除不再使用的变量
- [ ] `.cpp` 的 include 用**裸文件名**（不要带目录前缀）

### [ ] 7.5 文档

- [x] `Docs/UE5肉鸽射击_框架清单.md` 放回仓库 —— 修复 DevLog README 的 5 个死链
- [ ] 补开发日志：09-22、09-26、10-10 ~ 10-20
- [ ] 检查 `Docs/DevLog/README.md` 索引与实际文件是否一致

---

# 阶段二：肉鸽循环　**← 暂缓**

> **打完这一阶段才算「是肉鸽」。** 前 7 项只是射击游戏。

**暂缓原因**：玩法设计还没定。当前方向是「敌人类型 ↔ 玩家资源」，但细节（资源上限、溢出、消耗端）要先想清楚。

## [ ] 8. 肉鸽核心循环

**要做的事（旧方案，可能整体换掉）**：
- [ ] 击杀敌人 → 掉落收益（经验 / 金币 / 直接升级）
- [ ] 升级触发 → **三选一** 词条选择界面
- [ ] 词条库：至少 10 条，效果有差异
- [ ] 词条可叠加，效果能正确累加
- [ ] 词条来源走数据配置（DataAsset）
- [ ] 稀有度分级（普通/稀有/史诗），影响出现概率

**GAS 迁移后，词条的实现方式变了**：
> 词条不再"改数据资产的读取结果"，而是**施加一个 Duration / Infinite 的 GameplayEffect**。
> 例如「+20% 伤害」= 一个修改相关属性的 Infinite GE，叠层用 `Stacking`。

**验收标准**：
> 玩一局能明显感觉到「这局变强了」；同样的武器配不同词条，手感有可见差异。

**常见坑**：
- 词条效果写死在 UI 里 → 加一条词条要改五处
- 词条只做数值加成，没有质变型（如「子弹弹射」）
- 三选一出现重复/无意义的选项

---

# 阶段三：流程与 UI　**← 暂缓**

## [ ] 9. 关卡流程

**要做的事**：
- [ ] 开始界面 → 游戏 → 死亡结算 → 重开
- [ ] 用 `GameMode` / `GameState` / `PlayerState` 管理状态
- [ ] 中途可暂停
- [ ] 死亡后一键重开，且状态完全重置

**关键概念**：
- `GameMode` —— 规则 / `GameState` —— 全局状态 / `PlayerState` —— 玩家数据

**验收标准**：
> 连续重开 5 次，没有状态残留。

**常见坑**：
- 逻辑全写关卡蓝图 → 换关卡就没了
- 重开时忘了清理计时器 / 事件绑定 → 重开后行为异常

## [ ] 10. 基础 UI

**要做的事**：
- [ ] HUD：血量、弹药、当前波次、分数
- [ ] 升级三选一界面
- [ ] 结算界面
- [ ] 血条用 `ProgressBar`，**不要每帧更新**

**关键概念**：
- UMG 的**属性绑定（Binding）会每帧执行** → 改事件驱动
- `Widget` 不要直接 Cast 到角色拿数据，用事件广播

**验收标准**：
> UI 数据始终准确；打开 `stat game` 时 UI 没有明显的每帧开销。

---

# 跨阶段：手感

> **定位**：手感是「放大器」，不是「特色」。需要搭配一个一句话能说清的机制特色。
> **原则**：不为了手感停下主线；但**反馈接口必须早留**。

## 第一批 — 已完成（只留接口）

- [x] 建函数 `PlayAttackFeedback`（攻击方）/ `PlayHurtFeedback`（受击方）
- [x] 以后所有反馈（特效、震屏、飘字）都往这两个入口里加

> **GAS 迁移后的建议**：这两个入口最终应该换成 **GameplayCue**（按 Tag 自动触发，不用手动调）。

## 第二批 — 纯逻辑，随时可做

- [ ] **命中停顿（Hit Stop）**：命中时 `Set Global Time Dilation` 降到 0.1，持续 30~50ms 后恢复
      —— **打击感收益最大、成本最低的一项**
- [ ] **镜头后坐力**：开火 → 摄像机加 Pitch/Yaw 输入 + 自动回正
- [ ] **震屏**：`CameraShake` 资产，开火 / 命中各播一次
- [ ] **音效分层**：一次开火 = 主音 + 尾音 + 机械音

## 第三批 — 需要美术素材

- [ ] 第一人称手臂模型 + 动画（**依赖武器模型定型**）
- [ ] Niagara：枪口火焰、弹道、命中火花、血雾
- [ ] 真实音效替换占位音

## 第四批 — 最后

- [ ] 数值调优：后坐力大小、震屏强度、命中停顿时长、敌人反应速度

**验收标准**：
> 能录一段 10 秒视频，让人不看说明就能感到「打击很爽」。

---

# 阶段四：美术与音效替换　**← 暂缓**

> **时间预算：1 个月，不可压缩。** 灰盒项目在简历上吃亏。

- [ ] 替换角色模型和动画（Mixamo 可白嫖基础动画）
- [ ] 替换敌人模型
- [ ] 替换武器模型（M1911 / AK47 / 武士刀已导入）
- [ ] 枪口火焰、弹道、命中、爆炸特效（Niagara）
- [ ] 音效：射击、换弹、命中、受伤、UI、背景音乐
- [ ] **受击闪烁**（需要带参数的材质）
- [ ] 场景搭建，至少一个完整可玩的关卡
- [ ] 简单光照和后处理

**资源来源**：
- **Fab**（5.7 内置）—— 每月有永久免费的资源
- **Mixamo** —— 免费人形动画
- **Quixel / Megascans** —— 场景材质
- 引擎自带的基础音效库

**优先级**：音效 > 特效 > 模型。**因为手感提升最明显，成本最低。**

**第三方资源管理规则**：
> 第三方包**不进版本控制**（授权 + 体积），本地保留、编辑器里照常使用。
> 已在 `.gitignore` 排除：`Free_Sounds_Pack/` `M1911/` `Fab/` `SamuraiSword/` `Assets/`。
> 换机器后从 Fab 重新下载导入到同名目录即可。

---

# 阶段五：性能优化实战　**← P1**

> **这是简历上最值钱的部分。** 面试官要的是「发现 → 定位 → 解决 → 数据对比」的完整故事。

## [ ] 11. 建立性能观测能力

- [ ] 学会 `stat unit` / `stat game` / `stat gpu` / `stat fps`
- [ ] 学会用 **Unreal Insights** 抓取分析
- [ ] 能看懂：Game 线程 / Draw 线程 / GPU 谁先到瓶颈

**验收**：能对当前项目说出一句话诊断：「当前瓶颈在 Game 线程，主要来自 XX」。

## [ ] 12. 压力测试场景

- [ ] 用练枪关卡改造出压力测试场景
- [ ] 能一键把敌人数量拉到 200+
- [ ] 记录基准数据：不同敌人数量下的帧率、Game 线程耗时

**验收**：有一张自己的性能数据表，而不是「感觉挺流畅」。

## [ ] 13. 优化并记录

**常见优化点（按收益排序）**：

| 优化点 | 说明 |
|---|---|
| **Tick 滥用** | 关掉不需要每帧跑的 Tick，或改低频/定时器 |
| **SpawnActor 开销** | 改对象池 |
| **GAS 开销** | 检查 ActiveGameplayEffects 数量、Ability 的更新频率 |
| **Cast 频繁调用** | 缓存结果，或用接口 |
| **UMG 属性绑定** | 改事件驱动（本项目 HUD 已用事件驱动） |
| **物理与碰撞** | 简化碰撞体，减少重叠检测 |
| **Draw Call** | 合并材质、用实例化 |
| **AI 开销** | 降低行为树更新频率，远处敌人降频 |

- [ ] 每做一项优化，**记录优化前后的数据**
- [ ] 整理成：问题现象 → 定位过程 → 原因 → 解决方案 → 数据对比

**验收**：
> 能对着这份记录讲 5 分钟，包含具体数字。

---

# 玩法设计待定

> **现在只记录方向，不写进计划。** 等 P0 完成后细化。

**当前方向**：敌人类型 ↔ 玩家资源

```
绿色敌人 → Health
黄色敌人 → Ammo
蓝色敌人 → Energy
（金色 → Money，待定）
```

**必须先回答的问题**：

| # | 问题 | 为什么重要 |
|---|---|---|
| 1 | **资源上限要多低？** | 上限决定节奏。DOOM Eternal 上限极低，资源一直在流动 |
| 2 | **资源满了怎么办？** | 否则玩家会故意不打某种敌人 |
| 3 | **玩家能不能只打一种敌人？** | 必须让"无视某种敌人"付出代价 |
| 4 | **energy 的消耗端在哪？** | 没有出口的资源不是资源（需要先有技能） |
| 5 | **money 是不是该单独一层？** | 它是成长货币，不是战斗资源 |

**参考**：《DOOM Eternal》（不同击杀方式给不同资源）、[The aggressive resource management of Doom Eternal](https://www.gamedeveloper.com/design/the-aggressive-resource-management-of-i-doom-eternal-i-)。

---

# 附录 A：蓝图结构规范

> 目的：为后期转 C++ 铺路。**乱堆的蓝图转 C++ 等于重写。**

- [x] **少用关卡蓝图**，逻辑放 Actor / Component
- [x] **一个功能一个组件**
- [x] 用 **事件广播** 解耦，UI 和逻辑不要互相持有引用
- [~] 用 **接口** 通信，不用到处 Cast
- [x] 蓝图只做「连线和配置」，复杂计算留给 C++

**命名规范**：

| 前缀 | 类型 | 本项目现有 |
|---|---|---|
| `BP_` | 蓝图类 | `BP_Player` `BP_EnemyBase` `BP_GunBase` `BP_EnemySpawnPoint` |
| `WBP_` | UMG 控件 | `WBP_EnemyHealthBar` `WBP_MainUI` |
| `DA_` | DataAsset | `DA_Enemy_Melee` `DA_Enemy_Ranged` `DA_Weapon_*` |
| `DT_` | DataTable | — |
| `IA_` / `IMC_` | 输入 | `IA_Fire` `IA_Reload` `IA_Weapon1` `IA_Weapon2` |
| `E_` | 枚举 | `E_EnemyType` |
| **`AS_`** | **AttributeSet** | `AS_RushBattle`（C++） |
| **`GA_`** | **GameplayAbility** | `GA_Fire` `GA_Reload` |
| **`GE_`** | **GameplayEffect** | `GE_Damage` `GE_FireCooldown` `GE_InitStats` |
| `AIC_` | AI 控制器 | `AIC_Enemy` |
| `BB_` | Blackboard | `BB_Enemy` |
| `BT_` | Behavior Tree | `BT_Enemy_Melee` `BT_Enemy_Ranged` |
| `BTT_` / `BTD_` / `BTS_` | BT 任务 / 装饰器 / 服务 | `BTT_MeleeAttack` `BTT_RangedAttack` `BTS_UpdateCombatRange` |

**已知的命名欠账**：见 7.1。

---

# 附录 B：C++ 与蓝图的边界

**已经确立的分工：**

| 写 C++ | 写蓝图 |
|---|---|
| `UAttributeSet`（虚函数 + 宏只有 C++ 能用） | `UGameplayAbility`（逻辑调度） |
| 引擎没给蓝图开口子的部分（`InitAbilityActorInfo`、`AddSpawnedAttribute`） | UI、特效 |
| 性能敏感代码（以后） | 配置、关卡事件 |

**判断法则**：

> **C++ 只负责"引擎机制没给蓝图开的口子"，玩法逻辑尽量留蓝图。**

**实践记录（2026-10 接入 GAS 时踩的）**：

- 转 C++ 入口：`工具 → New C++ Class`
- `Build.cs` 必须加 `GameplayAbilities` / `GameplayTags` / `GameplayTasks`
- **改完 `Build.cs` 必须重新生成项目文件**（右键 `.uproject` → Generate Visual Studio project files），否则 VS 满屏红波浪线
- `.cpp` 里 include 自己的头文件用**裸文件名**，写全路径会报 `C1083`
- C++ 里 `CreateDefaultSubobject` 的 AttributeSet **会自动注册**（`InitializeComponent` 扫描 Owner 子对象）
- `InitAbilityActorInfo` 也会自动调（`Owner, Owner`）—— **但 ASC 挂 PlayerState 时 Avatar 会错，必须手动重设**

**什么该转 C++，什么留蓝图**：

| 留蓝图 | 转 C++ |
|---|---|
| UI | 核心玩法逻辑 |
| 特效 | 性能敏感代码 |
| 配置 | 需要复用的系统 |
| 关卡事件 | 数据结构和算法 |
| 原型验证 | |

---

# 附录 C：静默失败清单

> **UE 里最耗时的 bug：不报错、不中断，只是行为不对。**
> 排查必须看 `输出日志 (Output Log)`，不能只看画面。

## 蓝图 / 引擎层

| # | 类型 | 表现 | 怎么防 |
|---|---|---|---|
| 1 | `Cast` 失败 | 后面整条链不执行 | 接 `Cast Failed` 打印 |
| 2 | **派发器参数没接** | 广播出去的是 `null` / `0` | 接收方打印参数的显示名 |
| 3 | **空引用读取** | **只有日志一行红字，游戏照跑** | 出问题先看日志 |
| 4 | **绑定 ≠ 立即执行一次** | 数据对，但 UI 要等下一次广播 | 「切换当前对象」后手动刷一次 |
| 5 | **`BlackboardKeySelector` 未设** | 黑板写入静默失败 | 改用 `Get Blackboard` + 名字字符串 |

## GAS 层（2026-10 踩的）

| # | 坑 | 表现 | 解法 |
|---|---|---|---|
| 6 | **`Give Ability And Activate Once` 拒绝 `Local Only`** | 返回空 Handle，只打一条 Error | 用 `Give Ability` + `Try Activate Ability` |
| 7 | **冷却 GE 不授予 Tag** | **冷却完全无效** | 加 `Target Tags Gameplay Effect Component`（5.3 后标签搬到这里） |
| 8 | **`SetByCaller` 用错节点** | Modifier 被静默跳过，不扣血 | 必须用 **`Assign Tag Set By Caller Magnitude`**（引脚 `Data Tag`），不是 `Data Name` 那个 |
| 9 | **`Delay` 不可打断** | 技能取消后回调仍执行 | 用 `AbilityTask_WaitDelay` |
| 10 | **`InstancedPerExecution` 下靠"技能已激活"挡重复** | 挡不住 | 每次激活是新实例，**必须用 `Activation Owned Tags` + `Activation Blocked Tags`** |
| 11 | **`Make Outgoing Spec` 的目标忘了接** | 默认是 `self`（不是 ASC）→ 静默失败 | 用攻击者的 ASC |

---

# 附录 D：第三方资源与版本控制

| 项 | 规则 |
|---|---|
| 仓库 | GitHub 私有 |
| 二进制资产 | Git LFS（`.uasset` / `.umap` / 模型 / 贴图 / 音频） |
| 缓存目录 | `Saved/` `Intermediate/` `DerivedDataCache/` `Binaries/` 已忽略 |
| **第三方资源** | **不进版本控制**（授权 + 体积），本地保留 |
| 已排除 | `Content/Free_Sounds_Pack/` `Content/M1911/` `Content/Fab/` `Content/SamuraiSword/` `Content/Assets/` |
| 开发日志 | `Docs/DevLog/`，按日期，模板 `_TEMPLATE.md` |

**提交规范**：
```
类型: 一句话说清做了什么
feat / fix / refactor / perf / docs / chore
```
> **`perf` 提交必须带数字**（如 `Game 线程 18ms → 9ms`）——阶段五的优化履历靠它。

**提交前必看**：
```bash
git status          # 确认没有 Content/Assets/ 的东西混进来
```
