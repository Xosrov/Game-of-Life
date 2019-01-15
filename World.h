#ifndef WORLD_H
#define WORLD_H
#define FILLEDSQUARE "■ "
#define EMPTYSQUARE "□ "
enum Status
{
  Dead,
  Alive
};
class World
{
protected:
  unsigned width;
  unsigned height;
  Status **Blockstatus;
public:
  World(unsigned, unsigned, char);
  unsigned generation = 0;
  int seed;
  void Randomize();
  void Manual();
  void MapUpdate();
  void GameUpdate();
  ~World();
};
#endif