#include <vector>
#include <fstream>

using namespace std;

class BitUnpacker {
private:
    const vector<uint8_t>& data;

    size_t byte_pos = 0;

    uint64_t buffer = 0;
    int bits = 0;
    
public:
    BitUnpacker(const vector<uint8_t>& data)
        : data(data) {}

    bool unpack(uint64_t& value, int bit_count) {

        if (bit_count <= 0 || bit_count > 64)
            return false;

        value = 0;
        int value_bits = 0;

        while (value_bits < bit_count) {

            if (bits == 0) {

                if (byte_pos >= data.size())
                    return false;

                buffer = data[byte_pos++];
                bits = 8;
            }

            int take = min(bits, bit_count - value_bits);

            uint64_t mask;

            if (take == 64)
                mask = UINT64_MAX;
            else
                mask = (1ULL << take) - 1;

            uint64_t part = buffer & mask;

            value |= part << value_bits;

            buffer >>= take;
            bits -= take;

            value_bits += take;
        }

        return true;
    }

    uint64_t unpack(int bit_count) {

        uint64_t value;

        if (!unpack(value, bit_count))
            throw runtime_error("BitUnpacker: unexpected EOF");

        return value;
    }

    size_t remaining_bits() const {

        return (data.size() - byte_pos) * 8 + bits;
    }
};

int main(){
    vector<uint8_t> data;
    BitUnpacker unpacker(data);
    
    //decode x bits of data, and save them in 'd'.
    //the position of the data move x bits.
    int x;
    uint64_t d = unpacker.unpack(x);
    
    return 0;
}
