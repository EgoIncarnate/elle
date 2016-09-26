#ifndef ELLE_BARRIER_HXX
# define ELLE_BARRIER_HXX

namespace reactor
{
  template <typename E, typename ... Args>
  void
  Barrier::raise(Args&& ... args)
  {
    std::exception_ptr e;
    try
    {
      throw E(std::forward<Args>(args)...);
    }
    catch (...)
    {
      e = std::current_exception();
    }
    this->_raise(e);
    this->open();
    this->_raise(e);
  }

}

#endif