# RushBattle

一个用 **Unreal Engine 5.7** 边学边做的肉鸽（Roguelite）射击项目。

这个仓库除了放工程本身，还用来记录开发过程：每天的进展、踩过的坑、以及性能优化的前后数据对比。

---

## 技术栈

| 项目 | 值 |
|---|---|
| 引擎版本 | UE 5.7.3 |
| 项目类型 | 纯蓝图工程（后续计划渐进式转 C++） |
| 平台 | Windows 11 |
| 版本控制 | Git + Git LFS |

---

## 目录结构

```
RushBattle/
├─ RushBattle.uproject      项目入口文件
├─ Config/                  项目配置（引擎、输入、GameMode 等）
├─ Content/                 所有资产
│  ├─ Code/                 自己做的内容
│  │  ├─ Player/            玩家角色、GameMode、输入、UI
│  │  ├─ Enemy/             敌人类与生成器
│  │  ├─ Weapon/Gun/        武器
│  │  ├─ BluePointInterface/ 蓝图接口
│  │  └─ Map/               关卡
│  └─ Free_Sounds_Pack/     第三方音效包（⚠️ 不在仓库里，见下）
└─ Docs/DevLog/             开发日志
```

`Binaries/`、`Intermediate/`、`DerivedDataCache/`、`Saved/` 都是引擎可重建的缓存，
已被 `.gitignore` 排除，本地打开工程时会自动生成。

---

## 如何使用这个仓库

### 1. 必须先装 Git LFS

引擎的 `.uasset` / `.umap` 是二进制资产，本仓库用 Git LFS 管理。

```bash
git lfs install          # 每台机器只需要执行一次
git clone <仓库地址>
```

**如果不装 Git LFS 就直接 clone，拉下来的会是几 KB 的指针文件，而不是真正的资产。**

### 2. Free_Sounds_Pack 需要自己补

`Content/Free_Sounds_Pack/`（约 65 MB）是第三方音效包，**没有纳入版本控制**：
它体积占整个 Content 的 99%，且目前没有被任何蓝图引用。

因此 clone 之后，编辑器里的音效引用会是缺失状态。还原方式：

> 从 [Fab](https://www.fab.com/) 重新下载该音效包，导入到 `Content/Free_Sounds_Pack/` 下的同名目录结构即可。

### 3. 打开工程

用 UE 5.7 打开 `RushBattle.uproject`。首次打开会花一点时间重建缓存和编译 shader，属正常现象。

---

## 开发日志

开发日志在 [`Docs/DevLog/`](Docs/DevLog/) 目录下，按日期一篇。

- [日志索引](Docs/DevLog/README.md)
- [写作模板](Docs/DevLog/_TEMPLATE.md)

---

## 免责说明

本仓库包含的个人编写内容采用学习用途。仓库中若出现第三方资源，
其版权归原作者所有，不随本仓库授权。
