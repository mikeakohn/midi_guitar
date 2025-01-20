
class Serial
{
public:
  Serial();
  ~Serial();

  int init(const char *device);
  void destroy();

  uint8_t read_byte();

private:
  int fd;
  static struct termios oldtio;

};

