# SAMS V1.0
## 팀원
- 변정제
- 서요셉
- 석준형
- 오경석
- 윤덕근
- 이병두
  
## 프로젝트 개요
 대공유도탄 모의시스템은 공중위협에 따른 레이다, 유도탄, 발사대, 운용통제기에 대한 모의 상황을 부여
 하는 방식의 시스템으로 아래 그림과 같이 운용이 가능한 시스템이다.

## 파일 트리
### 공통 모듈
```
Common(공통모듈)
│
├─ UDPModule
│ └─ UDPModule.h
│ └─ UDPModule.cpp
│
└─ MessageParser
  └─ MessageParser.h
  └─ MessageParser.cpp

```
### 운용통제기
```
OperationController
│
├─ Scenario
│ └─ Scenario.h
│ └─ Scenario.cpp
│
├─ Event
│ └─ EventManager.h
│ └─ EventManager.cpp
│
├─ Display
│ └─ Display.h
│ └─ Display.cpp
│
├─ MessageFactory
│ └─ MessageFactory.h
│ └─ MessageFactory.cpp
└─ main.cpp
```
### 발사대모의기
```
LauncherSimulator
│
├─ LauncherControl
│ └─ LauncherControl.h
│ └─ LauncherControl.cpp
│
├─ LauncherSimulation
│ └─ LauncherSimulation.h
│ └─ LauncherSimulation.cpp
│
├─ MessageFactory
│ └─ MessageFactory.h
│ └─ MessageFactory.cpp
└─ main.cpp
```
### 유도탄모의기
```
MissileSimulator
│
├─ MissileControl
│ └─ MissileControl.h
│ └─ MissileControl.cpp
│
├─ MissileSimulator
│ └─ MissileSimulator.h
│ └─ MissileSimulator.cpp
│
├─ MessageFactory
│ └─ MessageFactory.h
│ └─ MessageFactory.cpp
└─ main.cpp
```
### 공중위협모의기
```
EnemySimulator
│
├─ EnemyControl
│ └─ EnemyControl.h
│ └─ EnemyControl.cpp
│
├─ EnemySimulation
│ └─ EnemySimulation.h
│ └─ EnemySimulation.cpp
│
├─ MessageFactory
│ └─ MessageFactory.h
│ └─ MessageFactory.cpp
└─ main.cpp
```
### 레이다모의기
```
RadarSimulation
│
├─ RadarControl
│ └─ RadarControl.h
│ └─ RadarControl.cpp
│
├─ RadarSimulation
│ └─ RadarSimulation.h
│ └─ RadarSimulation.cpp
│
├─ MessageFactory
│ └─ MessageFactory.h
│ └─ MessageFactory.cpp
└─ main.cpp
```
## 브랜치 전략
### 예상 1)
- 각 모의기별 브랜치를 생성하여 모의기 담당자들이 각 모의기 브랜치에서 개발을 진행하여 개별 테스트틀 진행한다.
- 통합 브랜치에서는 각 모의기의 브랜치를 병합하고 최신 브랜치로 만든 상태에서 빌드를 진행한 후 통합 테스트를 진행한다.

![SAMS-Branch](https://github.com/DSIC2024-TeamB5/SAMS/assets/56868159/f8a98479-89c5-4f8c-9874-d8789cdf2041)


### 예상 2)
- 모의기 별 레포지토리를 별도로 생성하여 개별 개발 진행한다.
