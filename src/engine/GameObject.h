#ifndef COH_GAMEOBJECT_H
#define COH_GAMEOBJECT_H

#include "gl/VAO.h"
#include "gl/Shader.h"
#include "col/PhysicsCollider.h"
#include "math/Vec3.h"
#include "render/RenderObject.h"
#include "render/MatrixManager.h"
#include "physics/Particle.h"
#include "physics/ParticleForceRegistry.h"
#include "physics/ParticleForceGenerator.h"

typedef struct GameObject_s GameObject;

/** The function callback for updates **/
typedef void FuncOnUpdate(GameObject* self, float tickDelta);
/** The function callback for collisions **/
typedef void FuncOnCollide(GameObject* self, GameObject* other);
/** The function callback for rendering **/
typedef void FuncOnRender(GameObject* self, float frameDelta, Shader* shader, MatrixManager* matMan);

typedef struct GameObject_s {
	/** The name of the object, used for identification **/
	char* name;
	/** The parent of the object. Used for object specific storage etc. **/
	void* parent;

	/** The position of the object in space. **/
	Vec3 position;
	/** The orientation of the object in space. **/
	Vec3 rotation;
	/** The size fo the object **/
	Vec3 scale;

	/** The speed in m/s the object is moving at. **/
	Vec3 velocity;
	/** The speed in m/s/s the object is accelerating at. **/
	Vec3 acceleration;
	/** The force in N the object is being acted upon with. **/
	Vec3 force;

	/** The collider of the object **/
	PhysicsCollider* physCollider;
	/** The physics particle of the object **/
	Particle* particle;
	/** The renderObject of the object **/
	RenderObject* render;

	/** The onUpdate callback of the object **/
	FuncOnUpdate* onUpdateCallback;
	/** The onCollide callback of the object **/
	FuncOnCollide* onCollideCallback;
	/** The onRender callback of the object **/
	FuncOnRender* onRenderCallback;

	//Game system info
	/** The pfRegistry to register force generators with. **/
	ParticleForceRegistry* pfRegistry;
} GameObject;

typedef struct GameObjectManager_s {
	GameObject*(* new)(char* name, void* parent, bool hasPhysics, bool hasRender, FuncOnUpdate* onUpdateCallback, FuncOnCollide* onCollideCallback, FuncOnRender* onRenderCallback, ParticleForceRegistry* pfRegistry);

	//Functions
	void(* update)(GameObject* gameObject, float tickDelta);
	void(* collide)(GameObject* gameObject, GameObject* other);
	void(* render)(GameObject* gameObject, float frameDelta, Shader* shader, MatrixManager* matMan);

	//Internals
	void(* setPhysicsCollider)(GameObject* gameObject, PhysicsCollider* collider);
	void(* setModel)(GameObject* gameObject, RenderObject* renderObject);

	//Positioning in space
	void(* setPositionXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* addPositionXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* setPositionVec)(GameObject* gameObject, Vec3* newPosition);
	void(* addPositionVec)(GameObject* gameObject, Vec3* displacement);

	void(* setScaleXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* addScaleXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* setScaleVec)(GameObject* gameObject, Vec3* scale);
	void(* addScaleVec)(GameObject* gameObject, Vec3* scale);

	void(* setRotationXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* addRotationXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* setRotationVec)(GameObject* gameObject, Vec3* rotation);
	void(* addRotationVec)(GameObject* gameObject, Vec3* rotation);

	//Physics
	void(* setVelocityXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* addVelocityXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* setVelocityVec)(GameObject* gameObject, Vec3* velocity);
	void(* addVelocityVec)(GameObject* gameObject, Vec3* velocity);

	void(* setAccelerationXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* addAccelerationXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* setAccelerationVec)(GameObject* gameObject, Vec3* acceleration);
	void(* addAccelerationVec)(GameObject* gameObject, Vec3* acceleration);

	void(* setForceXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* addForceXYZ)(GameObject* gameObject, scalar x, scalar y, scalar z);
	void(* setForceVec)(GameObject* gameObject, Vec3* force);
	void(* addForceVec)(GameObject* gameObject, Vec3* force);

	void(* addForceGenerator)(GameObject* gameObject, ParticleForceGenerator* forceGenerator);

	void(* delete)(GameObject* gameObject);
} GameObjectManager;

extern const GameObjectManager manGameObj;

#endif
