
 // Sample Program:
 // Compiler: gcc 3.2.3 20030502
 // Linux: Red Hat

 #include <execinfo.h>
 #include <sys/ucontext.h>
 #include <csignal>
 #include <cstdlib>

 #include <exception>
 #include <stdexcept>
 #include <vector>
 #include <sstream>
 #include <iostream>

 #define ERR_MESS(A) { std::cout << A << std::endl; }

 static struct sigaction oldh[32];

 typedef struct { char name[10]; int32_t id; char description[40]; } signal_def;

 signal_def signal_data[] =
  {
        { "SIGHUP   ", SIGHUP,    "Hangup (POSIX)" },
        { "SIGINT   ", SIGINT,    "Interrupt (ANSI)" },
        { "SIGQUIT  ", SIGQUIT,   "Quit (POSIX)" },
        { "SIGILL   ", SIGILL,    "Illegal instruction (ANSI)" },
        { "SIGTRAP  ", SIGTRAP,   "Trace trap (POSIX)" },
        { "SIGABRT  ", SIGABRT,   "Abort (ANSI)" },
        { "SIGBUS   ", SIGBUS,    "BUS error (4.2 BSD)" },
        { "SIGFPE   ", SIGFPE,    "Floating-point exception (ANSI)" },
        { "SIGKILL  ", SIGKILL,   "Kill, unblockable (POSIX)" },
        { "SIGUSR1  ", SIGUSR1,   "User-defined signal 1 (POSIX)" },
        { "SIGSEGV  ", SIGSEGV,   "Segmentation violation (ANSI)" },
        { "SIGUSR2  ", SIGUSR2,   "User-defined signal 2 (POSIX)" },
        { "SIGPIPE  ", SIGPIPE,   "Broken pipe (POSIX)" },
        { "SIGALRM  ", SIGALRM,   "Alarm clock (POSIX)" },
        { "SIGTERM  ", SIGTERM,   "Termination (ANSI)" },
        { "SIGSTKFLT", SIGSTKFLT, "Stack fault" },
        { "SIGCHLD  ", SIGCHLD,   "Child status has changed (POSIX)" },
        { "SIGCONT  ", SIGCONT,   "Continue (POSIX)" },
        { "SIGSTOP  ", SIGSTOP,   "Stop, unblockable (POSIX)" },
        { "SIGTSTP  ", SIGTSTP,   "Keyboard stop (POSIX)" },
        { "SIGTTIN  ", SIGTTIN,   "Background read from tty (POSIX)" },
        { "SIGTTOU  ", SIGTTOU,   "Background write to tty (POSIX)" },
        { "SIGURG   ", SIGURG,    "Urgent condition on socket (4.2 BSD)" },
        { "SIGXCPU  ", SIGXCPU,   "CPU limit exceeded (4.2 BSD)" },
        { "SIGXFSZ  ", SIGXFSZ,   "File size limit exceeded (4.2 BSD)" },
        { "SIGVTALRM", SIGVTALRM, "Virtual alarm clock (4.2 BSD)" },
        { "SIGPROF  ", SIGPROF,   "Profiling alarm clock (4.2 BSD)" },
        { "SIGWINCH ", SIGWINCH,  "Window size change (4.3 BSD, Sun)" },
        { "SIGIO    ", SIGIO,     "I/O now possible (4.2 BSD)" },
        { "SIGPWR   ", SIGPWR,    "Power failure restart (System V)" },
        { "SIGSYS   ", SIGSYS,    "Bad system call" },
  };

 using namespace std;

 /////////////////////////////////////////////

 class ExceptionTracer
 {
 public:
     ExceptionTracer( int32_t sig, siginfo_t * info, void * scp)
     {
         if (sig == SIGABRT)
         {
             signal(SIGABRT, SIG_DFL);
             exit(sig);
         }
 
         void * array[64];
         int32_t nSize = backtrace(array, 64);
         char ** symbols = backtrace_symbols(array, nSize);
         ucontext_t * ucx = (ucontext_t *)scp;

         signal_def * sdata = &signal_data[sig-1];

         ERR_MESS( "signal " << sig << " " << sdata->name << " " << sdata->id
                   << " " << sdata->description);

         ERR_MESS( "si_code " << info->si_code << " si_addr " << hex
                   << info->si_addr << dec << " ss_sp "
                   << ucx->uc_stack.ss_sp );

         for (int32_t i = 2; i < nSize; i++)
         {
             ERR_MESS( symbols[i] );
         }

         free(symbols);
         signal(SIGABRT, SIG_DFL);
         exit(sig);
     }
 };


class SignalTranslator : public exception
 {
 private:
     int32_t signum;
     class SingleTonTranslator
     {
     public:
         SingleTonTranslator( int32_t sig)
         {
             struct sigaction sa;
             sa.sa_sigaction = SignalHandler;
             sigemptyset (&sa.sa_mask);
             sa.sa_flags = SA_RESTART | SA_SIGINFO | SA_NODEFER | SA_RESETHAND;
             sigaction(sig, &sa, &oldh[sig]);
         }

         static void SignalHandler( int32_t sig, siginfo_t * info, void * scp)
         {
             throw ExceptionTracer( sig, info, scp);
         }

     };
 


 public:
     SignalTranslator( int32_t sig)
     {
          signum = sig;
          SingleTonTranslator s_objTranslator( sig);
          signal_def * sdata = &signal_data[sig-1];

          if (1) cout << "loaded handler for " << sdata->name << " ("
                      << sdata->id << "): " << sdata->description << endl;
     }
     ~SignalTranslator() throw()
     {
          int32_t sig = signum;
          sigaction( sig, &oldh[signum], NULL);
          signal_def * sdata = &signal_data[sig-1];

          if (1) cout << "unloaded handler for " << sdata->name << " ("
                      << sdata->id << "): " << sdata->description << endl;
     }
 };

 // SignalTranslator IllInstructionExceptionTr(SIGILL);
 // SignalTranslator TraceTrapExceptionTr(SIGTRAP);
 SignalTranslator AbortExceptionTr(SIGABRT);
 // SignalTranslator BusErrorExceptionTr(SIGBUS);
 SignalTranslator FloatingPointExceptionTr(SIGFPE);
 SignalTranslator SegmentationFaultTr(SIGSEGV);
 // SignalTranslator StackFaultExceptionTr(SIGSTKFLT);
 SignalTranslator CpuLimitExceptionTr(SIGXCPU);
 SignalTranslator FileSizeLimitExceptionTr(SIGXFSZ);
 // SignalTranslator PowerFailureExceptionTr(SIGPWR);
 // SignalTranslator SystemCallExceptionTr(SIGSYS);

 //////////////////////////////////////////////////////////////////////////

