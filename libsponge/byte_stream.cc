#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _queue(0) { _capacity = capacity; }

size_t ByteStream::write(const string &data) {
    size_t count = 0;
    for (auto c : data) {
        if (!remaining_capacity()) {
            break;
        }
        count++;
        _queue.push_back(c);
    }
    _write_count += count;
    return count;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    std::string peek;
    for (size_t i = 0; i < len; i++) {
        peek.push_back(_queue[_queue.size() - len + i]);
    }
    return peek;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    // if(len>_queue.size()){
    //    len = _queue.size();
    //}
    _read_count += len;
    for (size_t i = 0; i < len; i++) {
        _queue.pop_front();
    }
    if (buffer_empty() && input_ended())
        _eof = true;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    std::string read;
    size_t _buf_read = 0;
    for (size_t i = 0; i < len; i++) {
        _buf_read++;
        if (buffer_size() == 0) {
            if (input_ended()) {
                _eof = true;
            }
            break;
        }
        read.push_back(_queue.front());
        _queue.pop_front();
    }
    _read_count += _buf_read;
    return read;
}

void ByteStream::end_input() {
    _eoi = true;
    if (buffer_empty())
        _eof = true;
}

bool ByteStream::input_ended() const { return _eoi; }

size_t ByteStream::buffer_size() const { return _queue.size(); }

bool ByteStream::buffer_empty() const { return _queue.empty(); }

bool ByteStream::eof() const { return _eof; }

size_t ByteStream::bytes_written() const { return _write_count; }

size_t ByteStream::bytes_read() const { return _read_count; }

size_t ByteStream::remaining_capacity() const { return _capacity - _queue.size(); }
