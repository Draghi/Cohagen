#ifndef COH_GAMEOBJECTREGISTRY_H
#define COH_GAMEOBJECTREGISTRY_H

#include "GameObject.h"
#include "util/DynamicArray.h"
#include "render/MatrixManager.h"
#include "col/CollisionResolver.h"

typedef struct GameObjectRegist_s {
	/** The Shader to use when rendering.**/
	Shader* currentShader;
	/** The MatrixManager to use when rendering.**/
	MatrixManager* matMan;
	/** The ParticleForceRegist to register forces in.**/
	ParticleForceRegistry* pfRegistry;
	/** The list of objects that make up the world.**/
	DynamicArray* gameObjects;

	CollisionResolver* collisionResolver;
} GameObjectRegist;

typedef struct GameObjectRegistManager_s {
	/**
	 * Creates a new GameObjectRegist.
	 * @param matMan The prepared matrixManager to use.
	 * @return A new GameObjectRegist.
	 */
	GameObjectRegist*(* new)(MatrixManager* matMan);

	/**
	 * Adds the given gameobject to "the world"
	 * @param regist The GameObjectRegist to register with.
	 * @param gameObject The GameOBject to register.
	 */
	void(* add)(GameObjectRegist* regist, GameObject* gameObject);

	/**
	 * Changes what shader to render with.
	 * @param regist The GameObjectRegist to alter.
	 * @param shader The shader to use from now on.
	 */
	void(* setShader)(GameObjectRegist* regist, Shader* shader);

	/**
	 * Changes what matrixManager to render with.
	 * @param regist The GameObjectRegist to alter.
	 * @param matMan The MatrixManager to use from now on.
	 */
	void(* setMatrixManager)(GameObjectRegist* regist, MatrixManager* matMan);

	/**
	 * Gets the gameobject at the given ID.
	 * @param regist The registry to get the object from.
	 * @param id The id of the object to get.
	 * @return The gameobject at the given ID.
	 */
	GameObject*(* getGameObject)(GameObjectRegist* regist, int id);

	/**
	 * Updates and performs relevent physics operations to all registered objects.
	 * @param regist The registry to update.
	 * @param tickDelta The change in time between the last frame and now, in miliseconds.
	 */
	void(* update)(GameObjectRegist* regist, float tickDelta);

	/**
	 * Renders all registered objects.
	 * @param regist The registry to render.
	 * @param tickDelta The change in time between the last frame and now, in miliseconds.
	 */
	void(* render)(GameObjectRegist* regist, float frameDelta);


	/**
	 * Destroys the given registry..
	 * @param regist The registry to destroy.
	 *
	 * @todo Add cleanup code.
	 */
	void(* delete)(GameObjectRegist* regist);
} GameObjectRegistManager;

extern const GameObjectRegistManager manGameObjRegist;

#endif
