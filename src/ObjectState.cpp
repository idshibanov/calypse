#include "ObjectState.h"

#include <algorithm>


ActorState::ActorState() {
	_inv = make_shared<Inventory>();

	std::fill_n(_stats, (int)STAT_SCORE_LAST, 10);
	std::fill_n(_skills, (int)SKILL_SCORE_LAST, 1);
	
	_exp = 0;
	_hunger = 100;
	_run = false;
	
	recalcStats();
}

ActorState::~ActorState() {

}

shared_ptr<Inventory> ActorState::getInventory() const {
	return _inv;
}

int ActorState::getStat(StatScoreID id) const {
	return _stats[id];
}

int ActorState::getSkill(SkillScoreID id) const {
	return _skills[id];
}

void ActorState::recalcStats() {
	_hp = _stats[C_STAT_CON] * 4 + _stats[C_STAT_WILL] * 2 + _stats[C_STAT_STR];
	_mp = _stats[C_STAT_WILL] * 3 + _stats[C_STAT_INT];
	
	_stats[C_STAT_BEA] = (_stats[C_STAT_STR] + _stats[C_STAT_CON] + _stats[C_STAT_DEX]) / 3;
	_stats[C_STAT_CHA] = (_stats[C_STAT_INT] + _stats[C_STAT_WILL] + _stats[C_STAT_LUCK]) / 3;
}

int ActorState::getItemCount(int id) const {
	return _inv->getItemCount(id);
}

bool ActorState::useItem(int id) {
	return _inv->useItems(id, 3);
}

void ActorState::addItem(int id) {
	_inv->addItem(make_shared<Item>(id));
}

