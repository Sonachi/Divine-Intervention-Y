#pragma once

class State {
protected:
	void* self;
public:
	State(void*);
	~State();

	virtual void Execute() = 0;
	virtual void onEnter() = 0;
	virtual void onExit() = 0;
};

class StateMachine {
private:
	State* currentState;
public:
	StateMachine(State*);
	StateMachine();
	~StateMachine();

	void UpdateState();
	void ChangeState(State*);

	State * GetPreviousState();

private:
	State * previousState;
};