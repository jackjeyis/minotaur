#ifndef _MINOTAUR_LOOP_STAGE_H_
#define _MINOTAUR_LOOP_STAGE_H_
/**
 * @file event_loop_stage.h
 * @author Wolfhead
 */
#include "../common/logger.h"
#include <boost/noncopyable.hpp>
#include "event_loop_thread.h"

namespace minotaur { namespace event {

class EventLoopStage : public boost::noncopyable {
 public:
  EventLoopStage(uint32_t thread_count, uint32_t fd_size);
  ~EventLoopStage();

  int Start();
  int Stop();

  inline bool IsRunning() const {
    bool ret = false;
    for (const auto thread : event_loop_thread_) {
      ret |= thread->IsRunning();
    }
    return ret;
  }

  inline int RegisterRead(int fd, FdEventProc* proc, void* data) {
    return GetNotifier(fd).RegisterRead(fd, proc, data);
  }

  inline int RegisterWrite(int fd, FdEventProc* proc, void* data) {
    return GetNotifier(fd).RegisterWrite(fd, proc, data);
  }

  inline int RegisterClose(int fd) {
    return GetNotifier(fd).RegisterClose(fd); 
  }

  inline int UnregisterRead(int fd) {
    return GetNotifier(fd).UnregisterRead(fd);
  }

  inline int UnregisterWrite(int fd) {
    return GetNotifier(fd).UnregisterWrite(fd);
  }

  inline EventLoopNotifier& GetNotifier(int fd) {
    return event_loop_thread_[fd % event_loop_thread_.size()]->GetNotifier();
  }

 private:
  LOGGER_CLASS_DECL(logger);

  int CreateEventLoopThread();
  int DestoryEventLoopThread();

  uint32_t thread_count_;
  uint32_t fd_size_;
  std::vector<EventLoopThread*> event_loop_thread_; 
};

} //namespace event
} //namespace minotaur

#endif // _MINOTAUR_LOOP_STAGE_H_
