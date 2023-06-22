#pragma once

#include <QString>

#include "signalpage.h"

namespace fssp {

class SignalBuilder {
 public:
  SignalBuilder();

  static SignalPage *FromFile(const QString &absoluteFilePath,
                              const QString &fileExtension);

  class FileTypeError : public std::exception {
   public:
    virtual const char *what() const throw() {
      return "File type is not supported";
    }
  };
};

}  // namespace fssp