#ifndef SERVER_LINEBUFFER_H
#define SERVER_LINEBUFFER_H

#include <QByteArray>
#include <vector>

class LineBuffer {
public:
    void append(const QByteArray& chunk);
    std::vector<QByteArray> takeCompleteLines();
    void clear() { buffer_.clear(); }

private:
    QByteArray buffer_;
};

#endif //SERVER_LINEBUFFER_H