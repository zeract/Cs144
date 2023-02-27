#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    //DUMMY_CODE(seg);
    auto header = seg.header();
    auto pay = seg.payload();
    
    if(header.syn==true){
        _set_isn=true;
        isn = header.seqno;
    }
    if(_set_isn==true){
        _reassembler.push_substring(pay.copy(),unwrap(header.seqno,isn,_reassembler.last_reassemble())-1+(header.syn),header.fin);
    }
    
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    size_t ack = _reassembler.last_reassemble();
    if(_set_isn==false)
        return std::nullopt;
    else{
        if(_reassembler.stream_out().input_ended()==false){
            ack = ack+1;
        }else{
            ack = ack+2;
        }
        return std::optional<WrappingInt32>{wrap(ack,isn)};
    } 
    }

size_t TCPReceiver::window_size() const { return _capacity-_reassembler.stream_out().buffer_size(); }
