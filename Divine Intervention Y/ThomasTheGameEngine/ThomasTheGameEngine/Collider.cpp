#pragma once
#include "GameObject.h"
#include "Collider.h"

Collider::Collider(GameObject* _parent) : Component(_parent, Component::ComponentType::Collision){}

SphereCollider::SphereCollider(GameObject* _parent) : Collider(_parent){
	type = ColliderType::Sphere;
	collisionRadius = 1;
}

PlaneCollider::PlaneCollider(GameObject * parent_, Vec3 normal_) : Collider(parent_)
{
	type = ColliderType::BoundedPlane;
	plane = Plane(normal_, parent_->position);
}

bool Collider::isColliding(SphereCollider* _first, SphereCollider* _second){

	//if the length of the vector between the 2 centres is less than the 2 collision radius' return true, else false
	if (Vec3::length(_first->parentObject->position - _second->parentObject->position) < _first->collisionRadius + _second->collisionRadius){
		return true;
	} else {
		return false;
	}
}

bool Collider::isColliding(SphereCollider* _sphere, PlaneCollider* _plane){
	//Sum of interior angles is 2 * PI
	if (abs(_plane->plane.DistanceToPoint(_sphere->parentObject->position)) < _sphere->collisionRadius)
		return true;
	else
		return false;
}

bool Collider::isColliding(SphereCollider* _sphere, CubeCollider* _cube){ return false; }

bool Collider::isColliding(PlaneCollider* _plane, SphereCollider* _sphere){
	return Collider::isColliding(_sphere, _plane);
}

bool Collider::isColliding(PlaneCollider* _first, PlaneCollider* _second){
	if (Vec3::dot(_first->plane.normal,_second->plane.normal) == 0)
		return false;

	return false;
}
bool Collider::isColliding(PlaneCollider* _plane, CubeCollider* _cube){ return false; }

bool Collider::isColliding(CubeCollider* _cube, SphereCollider* _sphere){
	return Collider::isColliding(_sphere, _cube);
}

bool Collider::isColliding(CubeCollider* _cube, PlaneCollider* _plane){
	return Collider::isColliding(_plane, _cube);
}
bool Collider::isColliding(CubeCollider* _first, CubeCollider* _second){ return false; }

bool Collider::isColliding(Collider* _first, Collider* _second){
	
	//Delegate to the proper function call.
	//Don't touch unless yo know what yo doin'
	if (_first && _second)
	{
		if (_first->type == ColliderType::Sphere){
			if (_second->type == Collider::Sphere){
				return isColliding(((SphereCollider*)_first), ((SphereCollider*)_second));
			}
			else if (_second->type == ColliderType::BoundedPlane){
				return isColliding(((SphereCollider*)_first), ((PlaneCollider*)_second));
			}
			else if (_second->type == ColliderType::Cube){
				return isColliding(((SphereCollider*)_first), ((CubeCollider*)_second));
			}
		}
		else if (_first->type == ColliderType::BoundedPlane){
			if (_second->type == Collider::Sphere){
				return isColliding(((SphereCollider*)_second), ((PlaneCollider*)_first));
			}
			else if (_second->type == ColliderType::BoundedPlane){
				return isColliding(((PlaneCollider*)_first), ((PlaneCollider*)_second));
			}
			else if (_second->type == ColliderType::Cube){
				return isColliding(((PlaneCollider*)_first), ((CubeCollider*)_second));
			}
		}
		else if (_first->type == ColliderType::Cube){
			if (_second->type == Collider::Sphere){
				return isColliding(((SphereCollider*)_second), ((CubeCollider*)_first));
			}
			else if (_second->type == ColliderType::BoundedPlane){
				return isColliding(((PlaneCollider*)_second), ((CubeCollider*)_first));
			}
			else if (_second->type == ColliderType::Cube){
				return isColliding(((CubeCollider*)_first), ((CubeCollider*)_second));
			}
		}
	}
	else return false;

}