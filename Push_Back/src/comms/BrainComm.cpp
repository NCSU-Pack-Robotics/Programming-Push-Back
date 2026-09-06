#include "BrainComm.hpp"

BrainComm::BrainComm() {
    tx.flush();
    rx.flush();
}

size_t BrainComm::read(unsigned char* buf, const size_t count) {
    if (count == 0) return 0;
    if (buf == nullptr) return 0;

    const auto num_read = rx.read(buf, count);
    if (num_read <= 0 || num_read == PROS_ERR) return 0;

    return num_read;
}

void BrainComm::write(unsigned char* buf, const size_t count) {
    if (count == 0) return;
    if (buf == nullptr) return;

    tx.write(buf, count);
}
