
class InputDev
{
public:
  InputDev();
  ~InputDev();

  int init();
  void destroy();

  int update(int x, int y, int z);

private:
  int setup_abs(int fd, uint32_t channel, int min, int max);

  int fd;
  int curr_x, curr_y, curr_z;
};

