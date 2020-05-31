#pragma once

#include "AIComponents/PatrolComponents.h"
#include "AIComponents/NavigationComponents.h"
#include "AIComponents/FSMStateComponents.h"
#include "AttackComponents.h"

#include "SimpleComponents.h"

#define REGISTERED_TAGS \
    PlayerTag, BotTag, MainCameraTag
#define REGISTERED_COMPONENTS \
    MeleeAttackComponent, MeleeWeaponComponent, RangedAttackComponent, ThrowableAttackComponent, \
    GrenadeAttackComponent, GrenadeComponent, JumpSpeedComponent, SpeedComponent, \
    GravityComponent, HealthComponent, DecisionMakingComponent, NavMarginComponent, \
    PatrolmanComponent