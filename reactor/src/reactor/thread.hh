#ifndef INFINIT_REACTOR_THREAD_HH
# define INFINIT_REACTOR_THREAD_HH

# include <boost/signals2.hpp>
# include <boost/system/error_code.hpp>

# include <elle/Backtrace.hh>
# include <elle/named.hh>

# include <reactor/asio.hh>
# include <reactor/backend/fwd.hh>
# include <reactor/duration.hh>
# include <reactor/fwd.hh>
# include <reactor/signals.hh>
# include <reactor/waitable.hh>

namespace reactor
{
  typedef std::shared_ptr<Thread> ThreadPtr;

  class Thread:
    public Waitable
  {
  /*------.
  | Types |
  `------*/
  public:
    typedef Thread Self;
    typedef backend::Action Action;
    class Terminator
      : public std::default_delete<reactor::Thread>
    {
    public:
      void
      operator ()(reactor::Thread* t);
    };
    friend class Terminator;
    class unique_ptr
      : public std::unique_ptr<reactor::Thread, Terminator>
    {
    public:
      template <typename ... Args>
      unique_ptr(Args&& ... args);
    private:
      boost::signals2::scoped_connection _slot;
    };

  /*-------------.
  | Construction |
  `-------------*/
  public:
    Thread(Scheduler& scheduler,
           const std::string& name,
           const Action& action,
           bool dispose = false);
    Thread(const std::string& name,
           const Action& action,
           bool dispose = false);
    template <typename ... Args>
    Thread(const std::string& name,
           const Action& action,
           Args&& ... args);
    NAMED_ARGUMENT(dispose);
    NAMED_ARGUMENT(managed);
    // Returned shared ptr will be kept live until action finished
    static
    ThreadPtr
    make_tracked(Scheduler& scheduler,
                 const std::string& name,
                 const Action& action);
    // Returned shared ptr will be kept live until action finished
    static
    ThreadPtr
    make_tracked(const std::string& name,
                 const Action& action);
    virtual
    ~Thread();
  protected:
    /// Called by the scheduler when it doesn't reference this anymore.
    virtual
    void
    _scheduler_release();
  private:
    ThreadPtr _self;
    ELLE_ATTRIBUTE(bool, dispose);
    ELLE_ATTRIBUTE(bool, managed);

    /*---------.
    | Tracking |
    `---------*/
    public:
      typedef boost::signals2::signal<void ()> Tracker;
      // signal invoked when Thread object is being destroyed
      Tracker& destructed();
      // signal invoked when Thread is released by scheduler
      Tracker& released();
    private:
      Tracker _destructed;
      Tracker _released;

    /*-------.
    | Status |
    `-------*/
    public:
      struct state
      {
          enum State
          {
            /// The thread has finished.
            done,
            /// The thread is currently running.
            running,
            /// The thread is blocked on some condition or sleeping.
            frozen,
          };
      };
      typedef state::State State;
      /// Current state.
      State state() const;
      /// Whether our state is 'done'.
      bool done() const;
      /// Pretty name.
      std::string name() const;
      /// Debug dump.
      void Dump(std::ostream& output) const;
    private:
      State _state;

  /*----------.
  | Printable |
  `----------*/
  public:
    virtual
    void
    print(std::ostream& output) const override;

    /*----.
    | Run |
    `----*/
    public:
      void sleep(Duration d);
      void yield();
    private:
      void _step();

  /*-----------.
  | Exceptions |
  `-----------*/
  public:
    typedef boost::function<void ()> Injection;
    void inject(const Injection& injection);
    template <typename Exception, typename... Args>
    void raise(Args&&... args);
    void raise(std::exception_ptr e);
  protected:
    virtual
    void
    _action_wrapper(const Thread::Action& action);
  protected:
    boost::function<void ()> _injection;
    /// Exception to be thrown by the thread upon wake-up.
    std::exception_ptr _exception;
    friend class Exception;
    /// Exception thrown by the thread execution.
    std::exception_ptr _exception_thrown;

    /*----------------.
    | Synchronization |
    `----------------*/
    public:
      /** Wait for a group waitables to be done.
       *
       * If any of the specified waitables asks to be waited for,
       * this will freeze the thread until all of them are done. A
       * timeout may be specified, in which case the wait will be
       * aborted and false will be returned.
       *
       * @see Waitable.
       *
       * @param waitables The waitables to wait for.
       * @param timeout Maximum delay before aborting the operation.
       * @return Whether the operation finished and didn't timeout.
       */
      bool wait(Waitables const& waitables,
                DurationOpt timeout = DurationOpt());
      /// Shortcut to wait for a single waitable to be done.
      bool wait(Waitable& s,
                DurationOpt timeout = DurationOpt());
      /** Terminate execution of the thread by injecting a terminate exception.
       *
       *
       */
      void terminate();
      /** Terminate thread and wait for termination to be effective.
      * @param suicide: if true, terminate_now is a noop if this thread is the current one
      */
      void terminate_now(bool suicide = true);
    protected:
      virtual bool _wait(Thread* thread) override;
    private:
      friend class Waitable;
      friend class TimeoutGuard;
      void _wait_timeout(const boost::system::error_code& e,
                         std::string const& waited);
      void _wait_abort(std::string const& reason);
      void _freeze();
      void _wake(Waitable* waitable);
      std::set<Waitable*> _waited;
      bool _timeout;
      boost::asio::deadline_timer _timeout_timer;

  /*------.
  | Hooks |
  `------*/
  public:
    ELLE_ATTRIBUTE_RX(boost::signals2::signal<void ()>, frozen);
    ELLE_ATTRIBUTE_RX(
      boost::signals2::signal<void (std::string const&)>, unfrozen);

  /*--------.
  | Backend |
  `--------*/
  public:
    Scheduler& scheduler();
  private:
    friend class Scheduler;
    ELLE_ATTRIBUTE(std::unique_ptr<backend::Thread>, thread);
    ELLE_ATTRIBUTE(Scheduler&, scheduler);
    ELLE_ATTRIBUTE_R(bool, terminating);
  };

  template <typename R>
  class VThread: public Thread
  {
  /*---------.
  | Typedefs |
  `---------*/
  public:
    typedef VThread<R> Self;
    typedef boost::function<R ()> Action;

    /*-------------.
    | Construction |
    `-------------*/
    public:
      VThread(Scheduler& scheduler,
              const std::string& name,
              const Action& action);

    /*-------.
    | Result |
    `-------*/
    public:
      const R& result() const;
    private:
      R _result;
  };

  /*----------------.
  | Print operators |
  `----------------*/

  std::ostream& operator << (std::ostream& s, Thread::State state);
}

# include <reactor/thread.hxx>

#endif
