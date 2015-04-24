#pragma once

enum StatScoreID {
	C_STAT_STR = 0,
	C_STAT_CON,
	C_STAT_DEX,
	C_STAT_PER,
	C_STAT_INT,
	C_STAT_WILL,
	C_STAT_LUCK,
	C_STAT_BEA,
	C_STAT_CHA,
	STAT_SCORE_LAST
};
	
enum SkillScoreID {
	C_SKILL_MELEE = 0,
	C_SKILL_EXPLORE,
	C_SKILL_SURVIVAL,
	C_SKILL_FARM,
	C_SKILL_SMITH,
	C_SKILL_TAILOR,
	SKILL_SCORE_LAST
};

enum ItemID {
	C_ITEM_BRANCH = 0,
	C_ITEM_CONE,
	C_ITEM_APPLE,
	ITEM_ID_LAST
};


class ActorState {
	int _exp;
	int _hp;
	int _mp;
	int _hunger;

	bool _run;

	int _stats[STAT_SCORE_LAST];
	int _skills[SKILL_SCORE_LAST];
	int _inv[ITEM_ID_LAST];
	
	// private methods
	void recalcStats();
public:
	// functions
	ActorState();
	~ActorState();
	
	int getStat(StatScoreID id) const;
	int getSkill(SkillScoreID id) const;
	int getItemCount(ItemID id) const;
	bool useItem(ItemID id);
	void addItem(ItemID id);
};
