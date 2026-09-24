#include "protocolModels/lineBuffer/LineBuffer.h"

void LineBuffer::append(const QByteArray& chunk) {
    buffer_.append(chunk);
}

std::vector<QByteArray> LineBuffer::takeCompleteLines() {
    std::vector<QByteArray> lines;
    int pos = 0;
    while ((pos = buffer_.indexOf('\n')) != -1) {
        lines.push_back(buffer_.left(pos));
        buffer_.remove(0, pos + 1);
    }
    return lines;
}
