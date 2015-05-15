#include "ObjectState.h"

#include <algorithm>


ActorState::ActorState() {
	std::fill_n(_stats, (int)STAT_SCORE_LAST, 10);
	std::fill_n(_skills, (int)SKILL_SCORE_LAST, 1);
	std::fill_n(_inv, (int)ITEM_ID_LAST, 0);
	
	_exp = 0;
	_hunger = 100;
	_run = false;
	
	recalcStats();
}

ActorState::~ActorState() {

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

int ActorState::getItemCount(ItemType id) const {
	return _inv[id];
}

bool ActorState::useItem(ItemType id) {
	if(_inv[id] > 0) {
		_inv[id]--;
		return true;
	}
	return false;
}

void ActorState::addItem(ItemType id) {
	_inv[id]++;
}

