/**********************************************************************************
* \file         AllComponents.h
*
* \brief        Declaration of all components structure.
*
* \author       Darren Sim, Lee Jian Herng
*
* \email		    jinghengdarren.sim\@digipen.edu
*               jianherng.lee\@digipen.edu
*
* \date			    10 Oct 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*               or disclosure of this file or its contents without the prior
*               written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _ALL_COMPONENTS_
#define _ALL_COMPONENTS_

#include "../src/Graphics/include/Global.h"
#include <map>
#include "Shader.h"
#include "Circle.h"
#include "Triangle.h"
#include "Quad.h"
#include "Texture.h"
#include <vector>
#include <array>
#include <bitset>
#include "Particles.h"

struct b2JointEdge;
struct AudioComponent
{
	vec3 listenerPos;
	float listenPosOffsetX;
	vec3 sourcePos;
	vec2 emitterRange;
	vec3 posDisplacement;
	float volume = 0.0f;
	float maxVolume = 1.0f;
	int sfx = -1;
	int behaviourSelector = -1;
	float playDelay = 0.0f;
	float playingTime = playDelay;
	unsigned int id = 0;
	bool previewEmitter = true;
	bool isPlaying = false;
	bool canPlay = false;
	Direction listenerDirection;
	enum class Behaviours
	{
		None = 0,
		Counter
	};
	Behaviours myBehaviours;
};

struct CollisionData
{
	float PenetrationDepth = 0.0f;
	vec3 Normal;
	bool AreColliding = false;
	int colflag = 0;
};

struct IsActive
{
	bool isActive = true;
};

struct Transform
{
	Matrix3D matrix;
	vec3 position = { 0, 0, 0 };
	vec3 scale = { 40, 40, 0 };
	float rotation = 0.0f;
	Direction direction = Direction::Null;
};
enum class particleType
{
	None,
	Dust,
	Custom,
	MaxCount
};
struct Emitter
{
	int particleCounter = 0;
	int maxParticle = 0;
	float spawnRate = 0;
	float remaingTime = spawnRate;
	int particalSelector = -1;
	class Particle
	{
	public:
		vec2 vel;
		vec4 colorStart, colorEnd;
		float sizeStart, sizeEnd;
		float lifeTime = 1.0f;
		float rot = 0.0f;
		float deltaTimeMultiplier = 1.0f;
		float randomVariation1 = 1.0f;
		float randomVariation2 = 1.0f;
		bool setParticle = false;
		unsigned int ID;
		unsigned int particleLayer = 1;
		particleType my_particleType;
	};
	Particle my_particle;
};

struct ParticleProperties
{
	vec3 pos;
	vec2 vel;
	vec4 colorStart, colorEnd;
	float rot = 0.0f;
	float sizeStart, sizeEnd;
	float lifeTime = 1.0f;
	float LifeRemaining = 0.0f;
	float deltaTimeMultiplier = 1.0f;
	float randomVariation1 = 1.0f;
	float randomVariation2 = 1.0f;
	unsigned int ID;
	unsigned int particleLayer = 1;
	bool setParticle = false;
	bool status = false;
	particleType my_particleType;
};

enum class ProjectileType
{
	None,
	Boomerang,
	Pebble
};

enum class ProjectileState
{
	Idle,
	Holded,
	FlyingTo,
	Hit,
	ComingBack,
	Drop
};

struct Projectile
{
	ProjectileType type = ProjectileType::None;
	ProjectileState state = ProjectileState::Idle;
	vec3 origin, direction, target;
	float speed = 150.0f;
	float speedY = 150.0f;
	float speedUpTimer = 0.0f;
	bool hasTarget = false;
	vec3 spawnPoint;
	bool canTaunt = false;
};

struct Ui
{
  bool hasAnimation = false;
  bool playAnimation = false;
  bool isActive = false;
  int MainMenuselector = -1;
  int Levelselector = -1;
  vec2 isMovPos = {10000,100000};
  vec2 UiPos = { 0,0 };
  enum class pausemenu
  {
    None,
    HowtoPlay,
    Quit,
    Resume,
    HowtoPlayMenu,
    HowtoPlayPanel,
    HowtoPlayBack,
    HowtoPlayNxt,
    HowtoPlayClose,
    hp,
    progressbar,
    pausebg,
    suretoquitbg,
  	yes,
  	no,
    MaxCount
  };
  enum class mainmenu
  {
    None,
    Quit,
    Level1Play,
    yes,
    //yes or no to check to quit or not
    no,
    next,
    back,
    titlescreen,
    retry,
  	howtoplay,
  	close,
  	htpbg,
    MaxCount
  };
  pausemenu status = pausemenu::None;
  mainmenu Main = mainmenu::None;
  GLdouble timer = 0.0;
};

struct Sprite
{
	using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
	using textureIt = std::map<std::string, Texture>::iterator;
	using shaderIt = std::map<std::string, Shader>::iterator;

	glm::vec3 color{ 0.0f, 0.0f, 0.0f };
	glm::vec2 textureIdx{ 0.0, 0.0 }; //x for column index and y for row index 

	bool hasTexture = false;
	bool isQuad = false;

	glm::mat3 modelNDC_xform{ 0 };

	modelIt modelRef;
	textureIt textureRef;
	shaderIt shaderRef;

	unsigned int ID = 0;
	std::string name;

	unsigned int newLayer{ 0 };
	unsigned int layerNum{ 0 };

	float transparency{ 1.0f };

	bool triggerInstructionButtons = false;
	float instructionButtonsTimer = 1.0f;
};

struct BoundingBox
{
	AABB boundingBox = { {0,0,0} , {0,0,0} };
	vec3 boundingBoxSize{ 0,0,0 };
	CollisionType	_CollisionType = CollisionType::Null;
	int Collision_Flag = 0;
	bool CollisionCheck = false;
	bool knockback = false;
	float knockbackDuration = 0.0f;

	bool IsManual = false;
	float Offset_MaxX = 20.0f;
	float Offset_MinX = 20.0f;
	float Offset_MaxY = 20.0f;
	float Offset_MinY = 20.0f;

	// new 
	vec3 UpdatedPosition{ 0,0,0 };

	enum class LEVELNUMBER
	{
		None,
		TUTORIAL,
		LEVEL1,
		MAXCOUNT
	};

	LEVELNUMBER level = BoundingBox::LEVELNUMBER::None;
};

//Rock       Density : 0.6  Restitution : 0.1
//Wood       Density : 0.3  Restitution : 0.2
//Metal      Density : 1.2  Restitution : 0.05
//BouncyBall Density : 0.3  Restitution : 0.8
//SuperBall  Density : 0.3  Restitution : 0.95
//Pillow     Density : 0.1  Restitution : 0.2
//Static     Density : 0.0  Restitution : 0.4

struct RigidBody
{
public:
	Action _Action = Action::Null;
	BodyType _BodyType = BodyType::Null;
	CollisionType _CollisionType = CollisionType::Null;
	float mass = 1.0f;
	float InvertedMass = 1.0f;
	float gravity = 0; //9.78033f;
	vec4 force = { 0,0,0,0 };
	vec4 friction = { 0,0,0,0 };
	vec4 position = { 0,0,0,0 };
	vec3 velocitydirection = { 0,0,0 };
	vec4 acceleration = { 0,0,0,0 };
	float velocity = 1.0f;
	bool sliding = false;
	Action _Prev = Action::Null;
	float angle = 0.0f;

	struct Material
	{
		float density = 0.0f;
		float restitution = 0.0f;
	};

	// For forces
	float Timer_ForAttachedForce = 0.0f;
	float AttachedForce_Lifetime = 0.0f;

	//// rope
	b2JointEdge* m_jointList;
	int SquareNumber = 0;

	// Ultimate Hack
	CollisionData IndividualColData;

	// New Variables
	float restitution = 0.0f;
	float SpeedY = 1.0f;
	bool ClimbSliding = false;

};

struct BoundingSphere
{
	vec3 middlePoint = { 0,0,0 };
	float radius = 0.0f;
};

struct Plane
{
	vec3 normal;
	float _distance;
};

struct Animation
{
	enum class PlayerSheet
	{
		SwingRight,
		SwingLeft,
		JumpRight,
		JumpLeft,
		IdleRight,
		WalkRight,
		IdleLeft,
		WalkLeft,
		KnockbackRight,
		KnockbackLeft,
		Fainting,
		ThrowRockRight,
		ThrowRockLeft,
		ThrowBananaRight,
		ThrowBananaLeft,
		IdleRockRight,
		IdleRockLeft,
		IdleBananaRight,
		IdleBananaLeft,
		WalkRockRight,
		WalkRockLeft,
		WalkBananaRight,
		WalkBananaLeft,
		LeftClimb,
		RightClimb,
		UpClimb,
		JumpRockRight,
		JumpRockLeft,
		JumpBananaRight,
		JumpBananaLeft,
		TotalActions
	};

	enum class PlayerState
	{
		Idle,
		Walking,
		Jumping,
		Throwing,
		TotalStates
	};

	enum class Direction
	{
		Left,
		Right,
		TotalDirections
	};

	bool hasAnimation = false;
	bool playAnimation = false;

	GLdouble timer = 0.0;
	float frameTime = 0;
	unsigned currentCount = 0;
	unsigned int frameCount = 0;

	PlayerSheet PreviousAction = PlayerSheet::IdleRight;
	PlayerSheet CurrentAction = PlayerSheet::IdleRight;

	PlayerState PreviousState = PlayerState::Idle;
	PlayerState CurrentState = PlayerState::Idle;

	enum class EnemySheet
	{
		WalkLeft,
		WalkRight,
		AggroLeft,
		AggroRight,
		IdleLeft,
		IdleRight,
		KnockbackLeft,
		KnockbackRight,
		TotalActions
	};

	EnemySheet EnemyPreviousAction = EnemySheet::IdleLeft;
	EnemySheet EnemyCurrentAction = EnemySheet::IdleLeft;
};

struct Combat
{
	/* Struct containing attirbutes related to Combat
	*/
	struct Health
	{
		float Max_Health = 0;
		float Curr_Health = 0;
	} p_health;

	int damage = 0;
	AABB AttackAABB;
	EntityFlag Type_Of_Entity = EntityFlag::Null;

	double InvulTime = 0.0f;
	bool GodMode = false;
	bool IsRespawn = false;
	bool RespawnOnce = false;
	float InvulnerableTimer = 0.0f;
	bool PlayDeathAnimation = false;
	int DeathFrame = 0;
	bool FullTransparency = false;
	int TransparentCount = 0;

	std::bitset<1> AllowDeath;
};

struct Enemy
{
	EnemyType Enemy_Type = EnemyType::Normal;
	Direction Enemy_Direction = Direction::STAY;
	PatrolDirection _patrolDirection = PatrolDirection::P_STAY;
	bool AggroMoveCheck = false;
	bool AggroAttackCheck = false;
	bool PatrolCheck = false;

	float Timer = 0.0f;
	float MaxTimer = 0.0f;

	EntityState _EnemyState = EntityState::Idle;
	int CollisiongAttackFlag = 0;

	// new
	float HpBarTimer = 0.0f;
	Transform* chaseTarget = nullptr;

	/*************************************************************************
	Hit By Projectile
	0: Hit by Return
	1: Hit By Standard
	2: After Hit
	3: HP is all cleared
  *************************************************************************/
	std::bitset<4> IsHitByProjectile;

	float DeathTimer = 0.0f;
};

struct Motion {};

//struct _Animation
//{
//  vec2 TextureOffset = { 0 , 0 };
//
//  bool Is_Animation_playing = false;
//  bool DeathAnimationFlag = false;
//  bool PlayAnimationFlag = false;
//  float Timer = 0.0f;
//  float transparency;
//  Action PreviousAction = Action::Idle;
//  Action Action = Action::Idle;
//
//};

struct Object
{

};

struct Player
{
	EntityState _PlayerState = EntityState::Idle;
	int test = 0;
	int CollisiongAttackFlag = 0;
	Direction Player_Direction = Direction::STAY;

	int ThreeHits = 0;
	bool ThreeHitCombo = false;
	bool ThreeHitproc = false;

	// new
	float last_resort_timer = 0.0f;
	bool last_resort_flag = false;
	float ThreeHitComboTimer = 0.0f;
	bool attackflag = false;
	bool CrouchJump = false;
	float CrouchJumpTimer = 0.0f;
	bool airtime = false;
	float last_resort_last = 0.0f;
	bool _CheckPlatformThickness = false;
	int JumpCount = 0;

	bool ClimbFlag = false;

	ProjectileType projHoldingType = ProjectileType::None;
	unsigned int projHolding = 0;
	bool pickUpFlag = false;
	bool pickUpOneFlag = false;
	bool throwFlag = false;

	//Climbing
	bool ClimbThenGlide = false;
	bool HookOnTree = false;
	bool IsPushingBolder = false;
	int PushingBolderFlag = 0;

	// Flag Progress
	int ReceieveProgressFlagCounter = 0;

	// Bolder Check
	int LastPushedBolderID = 0;
	MapLocation _location;

	// Climb Animation timer
	float ClimbAnimationTimer = 0.0f;

};

struct Forces
{
	unsigned long Name;
	vec4 Force;
	float lifetime = 0.0f;
	bool Active = false;
	float maxlifetime = 0.0f;
};

struct Text
{
	std::string contents;
	glm::vec2 offset{ 0.0, 0.0 };
	glm::vec2 startingPos{ 0.0, 0.0 };
	glm::vec2 scale{ 0.0, 0.0 };
	glm::vec3 color{ 0.0, 0.0, 0.0 };

	unsigned int initialLength{ 0 };
};

struct Dialogue
{
	std::string contents;
	std::string originalContent;
	std::string updatedContent;
	glm::vec2 offset{ 0.0, 0.0 };
	glm::vec2 startingPos{ 0.0, 0.0 };
	glm::vec2 scale{ 0.0, 0.0 };
	glm::vec3 color{ 0.0, 0.0, 0.0 };
	double triggerPos{ 0.0 };
	double nextLineOffset{ 0.0 };
	unsigned int lineLength{ 0 };

	unsigned int currLine{ 0 };
	unsigned int currLength{ 0 };
	unsigned int nextLine{ 0 };
	unsigned int numChars{ 0 };
	double textSpeed{ 0.0 };
	double timer{ 0.0 };

	bool currentDialogue{ false };
	bool renderLine{ false };
	bool createDialogueBox{ false };
	unsigned int boxID{ 0 };
	unsigned int iconID{ 0 };
};

struct Camera
{
  glm::vec2 right{ 0 }, up{ 0 };
  glm::mat3 view_xform{ 0 };

  // window parameters 
  GLint height{ 0 };
  GLfloat ar{ 0.0f };
  glm::mat3 camwin_to_ndc_xform{ 0.0 };
  glm::mat3 world_to_ndc_xform{ 0.0 };

  // window change parameters
  GLint min_height{ 500 }, max_height{ 2000 };
  GLint height_chg_dir{ 1 };
  GLint height_chg_val{ 10 };
  GLfloat linear_speed{ 2.f };

  //COMMENTED OUT
  GLboolean camtype_flag{ GL_FALSE }; // button V
  GLboolean left_turn_flag{ GL_FALSE }; // button H
  GLboolean right_turn_flag{ GL_FALSE }; // button K
  GLboolean move_flag{ GL_FALSE }; // button U  

  // keyboard button press flags
  GLboolean zoom_flag{ GL_FALSE }; //button Z
  GLboolean zoomOut_flag{ GL_FALSE }; //button X
  GLboolean moveRight_flag{ GL_FALSE }; //button A
  GLboolean moveLeft_flag{ GL_FALSE }; //button D
  GLboolean moveUp_flag{ GL_FALSE }; //button W
  GLboolean moveDown_flag{ GL_FALSE }; //button S

  enum class CameraType
  {
    Free_Camera,
    Player_Camera,
    Cinematic_Camera
  };

  CameraType camType;

  glm::vec2 prevPos{ 0.0f };

  int eyeAlpha{ 1 };
  int eyeBeta{ 18 };
  int eyeRadius{ 40 };
};

struct Debug
{
	using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
	using shaderIt = std::map<std::string, Shader>::iterator;

	modelIt modelRef;
	shaderIt shaderRef;

	//RGBA
	glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
	glm::mat3 modelNDC_xform{ 0 };

	vec3 position = { 0, 0, 0 };
	vec3 scale = { 40, 40, 0 };
	float rotation = 0.0f;

};

struct Light
{
	using modelIt = std::map<std::string, std::unique_ptr<IModel>>::iterator;
	using shaderIt = std::map<std::string, Shader>::iterator;

	modelIt modelRef;
	shaderIt shaderRef;

	//RGBA
	glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f };
	glm::mat3 modelNDC_xform{ 0 };

	vec3 position = { 0, 0, 0 };
	float size = 0.0f;

};

struct Time
{
	double StartTime = 0.0;
	double EndTime = 0.0;
	double EndOfLoopTime = 0.0;
};

struct RopeTest
{

};

struct Logic
{
	bool knockback = false;
	float knockbackDuration = 0.0f;
	int flag = 0;
	bool IsDamaged = false;
};

struct Stick
{
	//int StickCount = 0;
	//int StickID = 0 ;
};

struct NUMBER
{
	bool _run = false;
	double health = 0.0f;

};

struct DeathZone
{
	bool ToggleDeathZone = false;

};

struct Alphabet
{
	bool _run = false;
	double health = 0.0f;

};

struct Portal
{
	int StageCounter = 0;
	float FlagUpAnimationTimer = 0.0f;
	bool FlagTouched = false;
	int counter = 0;

	/*************************************************************************
	Flag Status bitset layout
	0: Hoisting Flag in Tutorial
	1: Hoisted Flag in Tutorial
	2: Go To LEVEL1
	3: Hositing Flag in Level 1
	4: Hoisted Flag in level1
	5: Go to LEVEL2
  *************************************************************************/
	std::bitset<6> Flag_Status;
};

struct Taunt
{
	vec3 entityPoint = { 0,0,0 };
	vec3 centerPoint = { 0,0,0 };
	float radius = 0.0f;
	bool isActive = false;
};

struct HpBar
{
	int Follow_Enemy_ID = 0;
};

struct Environment
{
	bool HeldUp = true;
	int Hit = 0;
	bool HitOnce = false;
	bool AnimationDone = false;
	unsigned int boulderID = 0;

	// New Variables
	bool UpFlag = false;
	bool airtime = true;
	float Timer1 = 0.0f;
	float lifetime = 0.0f;
	float testduration = 0.0f;

	// Individual Collision Pushing Flag
	int PushFlag = 0;
	bool isItPushed = false;

	bool HiGroundOnce = false;
	bool RunDustEffect = false;

	bool cutVine = false;
};

struct Vines
{
	int BolderID = 0;
};

struct Climable
{
	enum class TypeOfTreeParts
	{
		Null,
		NormalTrunk,
		CurveTrunk,
		BranchLeft,
		BranchRight
	};

	int TrunkID = 0;
	TypeOfTreeParts Type = TypeOfTreeParts::Null;
	bool TouchingWall = false;
	bool toptrunk = false;

};

struct Buttons
{
	enum class ButtonType
	{
		Door,
		Null
	};

	int buttonID = 0;
	ButtonType Type = ButtonType::Null;
	bool isTriggered = false;
};

struct Bush
{
	enum class TypeOfBush
	{
		Null,
		BranchLeft,
		BranchRight
	};

	int BushID = 0;
	TypeOfBush Type = TypeOfBush::Null;
};

struct Door
{
	int doorID = 0;
	int buttons[5] = { 0 };
	bool fullyTriggered = false;
};

struct SpikeC
{
	unsigned int ID = 0;
	bool SpikeAnimationTimer = 0.0f;
	bool isDestory = false;
};

struct ReSpawnFlag
{
	unsigned int ID = 0;
	float FlagUpAnimationTimer = 0.0f;
	bool CurrentCheckPoint = false;
	vec3 NewCheckPopint = { 0,0,0 };
	int ProgressBarCounter = 0;

	bool PlayAnimation = false;
	int LotusIndex = 0;
};

struct Particle
{
    using textureIt = std::map<std::string, Texture>::iterator;
    using shaderIt = std::map<std::string, Shader>::iterator;
    using particleIt = std::map<std::string, Particles>::iterator;

    textureIt textureRef;
    shaderIt shaderRef;
    particleIt particleRef;

    enum class ParticleType
    {
        General,
        Leaves,
        Boulder,
        Mushroom,
		Portal,
		Health,
		Fire,
        TestParticles,
        TotalTypes
    };

    ParticleType type = ParticleType::General;

    glm::mat3 modelNDC_xform{ 0 };

    glm::vec2 position{ 0.0f, 0.0f };
    glm::vec2 scale{ 40.0f, 40.0f };
    glm::vec2 speed{ 0.0f, 0.0f };
    glm::vec4 color{ 0.0f, 0.0f, 0.0f, 1.0f }; //RGBA
    glm::vec2 UV{ 0.0f, 0.0f };
    glm::vec2 textureIdx{ 0.0, 0.0 }; //x for column index and y for row index 
    glm::vec2 gravity{ 0.0f, 0.0f };
    float weight{ 0.0f };
    float life{ -1.0f };

    glm::vec2 emitterPosition{ 0.0f, 0.0f };

    float spread = 0.0f;
    glm::vec2 maindir{ 0.0f, 0.0f };
    glm::vec2 randomdir{ 0.0f, 0.0f };

    unsigned int newParticles = 0;

    bool absParticles = false;
    float timer = 1.0f;
    float interval = 0.0f;
    unsigned int counter = 0;
    unsigned int particlesPerInterval = 0;

    bool hasTexture = false;
    bool isTriggered = false;

    //Test variables (Now needed)
    glm::vec2 maindirMinRange{ 0.0f };
    glm::vec2 maindirMaxRange{ 0.0f };
    glm::vec2 randomdirMinRange{ 0.0f };
    glm::vec2 randomdirMaxRange{ 0.0f };
    glm::vec2 spreadRange{ 0.0f };
    glm::vec2 lifeFactor{ 0.0f };
    glm::vec2 initEmitterPos{ 0.0f };
    glm::vec2 emitterVarMin{ 0.0f };
    glm::vec2 emitterVarMax{ 0.0f };
    glm::vec4 gravityVar{ 0.0f };
    float initLife{ -1.0f };
    glm::vec2 startScale{ 40.0f, 40.0f };
    glm::vec2 endScale{ 40.0f, 40.0f };
    glm::vec4 endColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    glm::vec4 startColor{ 1.0f, 1.0f, 1.0f, 1.0f };

	float angle{ 0.0f };
	float angleSpeed{ 0.0f };
	glm::vec2 radius{ 0.0f };

    unsigned int ID = 0;

	bool fallingBoulder = false;
	unsigned int numIterations = 0;
};

struct CenterPoints
{
	glm::vec2 position{ 0.0f, 0.0f };
	float radius = 0.0f;
	bool isTriggered = false;

    float timer = 1.0f;

    glm::vec2 scale{ 0.0f, 0.0f };
    AABB aabb { {0.0f,0.0f,0.0f} , {0.0f,0.0f,0.0f} };

    bool fixHorizontalAxis = false;
    bool lockVerticalAxis = true;
};

struct Platforms
{
	enum class PlatformType
	{
		None,
		BOUNCEUP,
		SHOOTING,
		DISAPPEARING,
		MAXCOUNT,
	};

	enum class Direction
	{
		None,
		LEFT,
		RIGHT,
		MAXCOUNT,
	};

	unsigned int ID = 0;

	// Universal 
	PlatformType _Type = PlatformType::None;
	bool SwitchOn = false;

	// Bounce Up
	bool push = false;
	bool reset = false;
	float BounceTimer = 0.0f;
	bool AllowPush = true;
	float ForceY = 1450.0f;
	float BounceAnimation = 0.0f;
	bool reverse = false;
	int CounterFrame = 0;

	// Shooting Platform
	float DelayTimer = 0.0f;
	float StoreDelayTimer = 0.0f;
	Direction AimDirection = Direction::None;
	float ForceX = 50.0f;
	float MaxDistance = 999.0f;
	bool ShootAlready = false;
	float ProjectileLifetime = 0.0f;
	float ShootAnimationTimer = 0.0f;
	bool CanShoot = false;
	int Count = 0;
	vec3 FiringPosition = { 0.0f,0.0f,0.0f };
	bool SetFiringPosition = false;

	//Disappearing Platform
	bool Disappear = false;
	bool ReAppear = false;
	float respawnTimer = 0.0f;
	float disappearTimer = 0.0f;
	float StorerespawnTimer = 0.0f;
	float StoredisappearTimer = 0.0f;
	bool StepOnce = false;
	bool Appear = false;
	bool SET = false;
	float DisappearAnimationTimer = 0.0f;
	float ReappearAnimationTimer = 0.0f;
	int IndexCounter = 0;
	int ForestOrCave = 0;

};

struct PlatformProjectiles
{
	unsigned int ID = 0;
	float lifetime = 0.0f;
	float Distance = 0.0f;
	bool CalcultedDistance = false;
	vec3 StartPosition{ 0.0f,0.0f,0.0f };
	vec3 EndPosition{ 0.0f,0.0f,0.0f };
};

struct LoadingScreenTemp
{
	enum class Type
	{
		NONE,
		BACKGROUND,
		MONKEY,
		CAVEBACKGROUND,
		NEXTAREAISCAVES,
		CAVEHOLE,
		MAXCOUNT
	};

	unsigned int ID = 0;
	float FadeAwayTimer = 0.0f;
	Type _Type = LoadingScreenTemp::Type::NONE;
	int FadeCounter = 0;
	float AnimationTimer = 0.0f;
};

// Component to register for COlliison and pHysics 
struct RegisterEvent
{
  /*************************************************************************
  Register Collision bitset layout
  0: UnRegistered.
  1: Registered.
  2: Locked
*************************************************************************/
  std::bitset<3> RegisterPhysics;
  std::bitset<3> RegisterCollision;

  bool IsCOLLRegistered = false;
  bool IsPHYRegistered = false;
};

#endif // !_ALL_COMPONENTS_

// Pls ignore 