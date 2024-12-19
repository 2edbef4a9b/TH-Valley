#include "sample/sample_application.h"

int main(int argc, char** argv) {
    const th_valley::SampleApplication app;
    return th_valley::SampleApplication::getInstance()->run();
}
