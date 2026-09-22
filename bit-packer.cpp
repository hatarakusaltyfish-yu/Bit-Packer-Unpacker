#include <vector>
#include <fstream>

using namespace std;

class BitPacker {
    private:
        std::vector<uint8_t> data;
        uint64_t buffer = 0;
        int bits = 0;

    public:

        void pack(uint64_t value, int bit_count) {

            while (bit_count > 0) {

                int free_bits = 64 - bits;
                int take = min(bit_count, free_bits);

                uint64_t mask;

                if (take == 64)
                    mask = UINT64_MAX;
                else
                    mask = (1ULL << take) - 1;

                value &= mask;

                buffer |= value << bits;
                bits += take;

                value >>= take;
                bit_count -= take;

                while (bits >= 8) {

                    data.push_back(
                        static_cast<uint8_t>(buffer & 0xFF)
                    );

                    buffer >>= 8;
                    bits -= 8;
                }
            }
        }

        void flush() {

            if (bits > 0) {

                data.push_back(
                    static_cast<uint8_t>(buffer & 0xFF)
                );

                buffer = 0;
                bits = 0;
            }
        }

        const std::vector<uint8_t>& get_data() const {
            return data;
        }
};

int main(){
    //define a packer
    BitPacker packer;
    
    //packing data, using *bit_count* bits to save *value*
    uint64_t value=5;
    int bit_count=3;
    packer.pack(value, bit_count);
    
    //**MUST** flush after packing data
    packer.flush();
    
    //write data into file
    ofstream OutFile("file.dat", ios::out | ios::binary);
    const auto& data = packer.get_data();

    for (uint8_t byte : data) {
        OutFile.write(
            reinterpret_cast<char*>(&byte),
            sizeof(byte)
        );
    }
}
