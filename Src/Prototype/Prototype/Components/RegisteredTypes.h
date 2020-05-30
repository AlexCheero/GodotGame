#pragma once

#include "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/AIComponents/PatrolComponents.h"
#include "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/AIComponents/NavigationComponents.h"
#include "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/AIComponents/FSMStateComponents.h"
#include "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/AttackComponents.h"

#include "C:/Users/Alex/Documents/GodotProjects/Projects/Prototype/Src/Prototype/Prototype/Components/SimpleComponents.h"

#define REGISTERED_TAGS \
    PlayerTag, BotTag, MainCameraTag
#define REGISTERED_COMPONENTS \
    MeleeAttackComponent, MeleeWeaponComponent, RangedAttackComponent, ThrowableAttackComponent, \
    GrenadeAttackComponent, GrenadeComponent, JumpSpeedComponent, SpeedComponent, \
    GravityComponent, HealthComponent, DecisionMakingComponent, NavMarginComponent, \
    PatrolmanComponent