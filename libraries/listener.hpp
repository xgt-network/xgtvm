#pragma oonce

class listener
{
  public:
  virtual ~listener() {}
  virtual void pre_word(machine& a, word& i) = 0;
  virtual void post_word(machine& a, word& i) = 0;
};

class stub_listener : public listener
{
  public:
  ~stub_listener() {}
  void pre_word(machine& a, word& i)
  {
    std::cout << "stub_listener::pre_word" << std::endl;
  }
  void post_word(machine& a, word& i)
  {
    std::cout << "stub_listener::post_word" << std::endl;
  }
};
