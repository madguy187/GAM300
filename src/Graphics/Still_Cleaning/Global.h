#ifndef GLOBAL_H
#define GLOBAL_H
#include "pch.h"
#include "Math_Lib.h"

enum class EntityFlag
{
  Null = 0, // platforms or anything else
  Player,
  Enemy,
  MaxCount,
};

enum class Component
{
  Null = 0,
  Transform,
  Motion,
  Sprite,
  Animation,
  Collision,
  MaxCount,
};

enum class Direction
{
  Null = 0,
  STAY,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  MaxDirections,
};

enum class Action
{
  Null = 0,
  Idle,
  WalkingRight,
  WalkingLeft,
  Jump,
  JumpingDown,
  CLIMBING,
  SLIDINGDOWN,
  STOPCLIMBING,
  MaxActions,
};

struct AABB
{
  vec3 min = { 0,0,0 };
  vec3 max = { 0,0,0 };
};

enum class EnemyType
{
  Null = 0,
  Normal,
  Deer,
  Snake,
  MaxEnemies,
};

enum class BodyType
{
  Null = 0,
  Dynamic,
  Kinematic,
  Static,
  MaxCount
};

enum class CollisionType
{
  Null = 0,
  Solid,  //not passable on all side
  Ghost,  // fully passable
  Node,
  Enemy,
  Player,
  Stick,
  DeathZone,
  Taunt,
  Flag,
  Projectile,
  Bolder,
  Vines,
  Climbable,
  Bush,
  Spikes,
  BOUNCEUP,
  PLATFORMPROJECTILES,
  SHOOTINGPLATFORM,
  DISAPPEARINGPLATFORM,
  Maxcount

};

enum class EntityState
{
  Idle,
  Attack,
  Death,
  Move,
  Patrol,
  Ulti,
  Locked,
  IsTaunted
};

enum class PatrolDirection
{
  P_STAY = 0,
  P_LEFT,
  P_RIGHT,
};

enum class ContainerType
{
  Null = 0,
  TUTORIALLEVELCOLLISION = 1,
  LEVEL1COLLISION = 2,
  LEVEL2COLLISION = 3,
  TUTORIALPHYSICS = 4,
  LEVEL1PHYSICS = 5,
  LEVEL2PHYSICS_Part1 = 6,
  LEVEL3PHYSICS = 7,
  LEVEL2PHYSICS_Part2 = 8,
  MAXCOUNT

};

// Enum class for location

enum class MapLocation
{
  FirstStart = 0,
  TUTORIAL = 1,
  LEVEL1 = 2,
  LEVEL2_PART1 = 3,
  LEVEL2_PART2 = 4,
  LEVEL3_PART1 = 5,
  LEVEL3_PART2 = 6,
  MAXCOUNT
};

const int COLLISION_LEFT = 0x00000001;    //0001 
const int COLLISION_RIGHT = 0x00000002;    //0010 
const int COLLISION_TOP = 0x00000004;    //0100 
const int COLLISION_BOTTOM = 0x00000008;    //1000

#endif