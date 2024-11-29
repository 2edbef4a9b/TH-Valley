#include "sample/sample_application.h"

int main(int argc, char** argv) {
    const SampleApplication app;
    return SampleApplication::getInstance()->run();
}
