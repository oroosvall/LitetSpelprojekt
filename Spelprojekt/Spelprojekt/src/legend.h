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
	stalagmite,
	minecart,
	minetrack,
	minestruct_small,
	minestruct_large,
	lantern,
	platformRock1x1,
	platformRock1x3,
	platformRock1x5,

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

	enemy_count, //leave this last
};

#endif