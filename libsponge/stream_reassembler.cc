#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    //DUMMY_CODE(data, index, eof);
    if(data.length()!=0){
        if(eof){
        _eof=true;
        _eof_index = index+data.length()-1;
        }
        if((index+data.length())>_current_index&&index<=_current_index){
                size_t write = _output.write(data.substr(_current_index-index,data.length()+index-_current_index));
                _current_index = _current_index+write;
                size_t size = _store.size();
                for(size_t i=0;i<size;i++){
                    int count=0;
                    for(auto str:_store){
                        if((str.index+str._queue.length())>=_current_index&&str.index<=_current_index){
                            size_t add=_output.write(str._queue.substr(_current_index-str.index,str._queue.length()+str.index-_current_index));
                            _current_index = _current_index+add;
                            _store.erase(_store.begin()+count);
                            break;
                        }
                        if((str.index+str._queue.length())<=_current_index&&str.index<=_current_index){
                            _store.erase(_store.begin()+count);
                            break;
                        }
                        count++;
                    }
                }
                
        }

        if(index>_current_index){
            _store.push_back({index,data});
        }
        if(_eof&&_current_index-1==_eof_index){
            _output.set_eof();
            _output.end_input();
        }
    }else{
        if(eof){
            _eof=true;
            _eof_index = index;
            _output.end_input();
        }
    }
    
    
}

size_t StreamReassembler::unassembled_bytes() const { 
    if(_store.empty()){
        return 0;
    }
    
    size_t min = _store[0].index;
    size_t max = _store[0].index+_store[0]._queue.length();
    vec.push_back({min,max});
    for(auto str:_store){
        if(str.index<min){
            min = str.index;
        }
        if(str.index+str._queue.length()>max){
            max = str.index+str._queue.length();
        }
    }
    if(min<_current_index){
        min = _current_index;
    }
    return max-min; 
    }

size_t StreamReassembler::ackno()const{
    if(_store.empty()){
        return 0;
    }
    size_t min = _store[0].index;
    for(auto str:_store){
        if(str.index<min){
            min = str.index;
        }
    }
    if(min<_current_index){
        min = _current_index;
    }
    return min;
}
bool StreamReassembler::empty() const { return _store.empty(); }
