#ifndef LEGEND_H
#define LEGEND_H

enum OBJ
{
	BACK,
	WORLD,
	MISC,
	ENEMY,
};

enum BackID
{
	grid,
	rock_grey,
	rock_dirt,
	rock_boss,

	back_count, //leave this last
};

enum WorldID
{
	box,
	mushroom,
	stalactite,
	Rock1x1,
	minecart,
	minetrack,
	minestruct_small,
	minestruct_large,
	lantern,
	platformRock1x1,
	platformRock1x3,
	platformRock1x5,
	platformRock3x3,
	platformRock5x5,
	Rock5x5,
	Rock5x5Crack,
	MushroomWall,
	Pickaxe,
	Board,
	CrateHD,
	Crate3D,
	platformMarble1x1,
	platformMarble1x3,
	platformMarble1x5,
	platformMarble3x3,
	platformMarble5x5,

	world_count, //leave this last
};

enum MiscID
{
	shrine,
	rune_range,
	rune_damage,
	rune_shield,

	misc_count, //leave this last
};

enum EnemyID
{
	spikes,
	bat,
	flame,
	cube,
	spider,
	ghost,
	web,

	enemy_count, //leave this last
};

enum EffectType
{
	NONE,
	torch,
	spark,
};

#endif