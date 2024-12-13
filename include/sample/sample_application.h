#ifndef SAMPLE_APPLICIATION_H_
#define SAMPLE_APPLICATION_H_

#include "axmol.h"

namespace th_valley {

class SampleApplication : private ax::Application {
public:
    void initGLContextAttrs() override;

    bool applicationDidFinishLaunching() override;
    void applicationDidEnterBackground() override;
    void applicationWillEnterForeground() override;
};

}  // namespace th_valley

#endif  // SAMPLE_APPLICIATION_H_
