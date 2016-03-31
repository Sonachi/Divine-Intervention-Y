#include "States.h"

State::State(void* whoAmI){
	self = whoAmI;
}
State::~State(){}

StateMachine::StateMachine(State* baseState_){
	currentState = baseState_;
}

void StateMachine::UpdateState(){
	if (currentState)
		currentState->Execute();
}

void StateMachine::ChangeState(State* newState){
	if (currentState) {
		currentState->onExit();

		if (newState != previousState)
			delete previousState;

		previousState = currentState;

		//delete currentState;
	}

	if (newState) {
		currentState = newState;
		currentState->onEnter();
	}
}

StateMachine::~StateMachine(){
	if (currentState) delete currentState;
	if (previousState) delete previousState;
}

StateMachine::StateMachine(){}

State * StateMachine::GetPreviousState()
{
	return previousState;
}