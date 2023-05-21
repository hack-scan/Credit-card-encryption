#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "calculator.h"

#define BIN_LEN 6

std::vector<std::string> BIN = {
    "402658", "436742", "519413", "527061", "436718", "451290", "489592",
    "491031", "403361", "404117", "439227", "468203", "622202", "622203",
    "621793", "622421", "622150", "622151", "622181", "622188", "356833",
    "356835", "409666", "409668", "403360", "427020", "427030", "427040",
    "438589", "439225", "439226", "451289", "427570", "427571", "472067",
    "472068", "622526", "622527", "622588", "622598", "622521", "622522",
    "622523", "622525", "622519", "622520", "622516", "622518", "622622",
    "622623", "622625", "622628", "621658", "621659", "621660", "621661",
    "521899"
};

std::unordered_map<std::string, int> g_bin_index_mp;

bool is_valid_credit_card_number(const std::string& credit_card_number) {
    int size = credit_card_number.size();
    if (size < 13 || size > 19) {
        return false;
    }
    int odd_weight = 2, even_weitht = 1;
    if (size % 2 != 0) {
        odd_weight = 1, even_weitht = 2;
    }
    int check_sum = 0;
    for (int i = 0; i < size; ++i) {
        int cur_num = 0;
        if ((i + 1) % 2 != 0) {
            cur_num = (credit_card_number[i] - '0') * odd_weight;
        } else {
            cur_num = (credit_card_number[i] - '0') * even_weitht;
        }
        if (cur_num > 9) {
            cur_num -= 9;
        }
        check_sum += cur_num;
    }
    return check_sum % 10 == 0;
}

int calc_check_code(const std::string& credit_card_number) {
    int size = credit_card_number.size();
    int odd_weight = 2, even_weitht = 1;
    if (size % 2 != 0) {
        odd_weight = 1, even_weitht = 2;
    }
    int check_sum = 0;
    for (int i = 0; i < size; ++i) {
        int cur_num = 0;
        if ((i + 1) % 2 != 0) {
            cur_num = (credit_card_number[i] - '0') * odd_weight;
        } else {
            cur_num = (credit_card_number[i] - '0') * even_weitht;
        }
         if (cur_num > 9) {
            cur_num -= 9;
        }
        check_sum += cur_num;
    }

    return ((check_sum / 10 + 1) * 10 - check_sum) % 10;
}

std::string encrypt(const std::string& credit_card_number, int solution = 1) {
    if (!is_valid_credit_card_number(credit_card_number)) {
        printf("[ERROR] credit card number illegal, length or check sum fail!\n");
        return std::string();
    }

    int size = credit_card_number.size();
    int account_id_len = size - (BIN_LEN + 1);

    std::string bank_id = credit_card_number.substr(0, 6);
    std::string account_id_prefix = credit_card_number.substr(6, 6);
    std::string account_id_suffix = credit_card_number.substr(12, account_id_len - 6);
    std::string last_4_num = credit_card_number.substr(size - 4, 4);
    std::string adjust_num = credit_card_number.substr(0, 2) + last_4_num;

    std::string encry_num;
    for (int i = 0; i < adjust_num.size(); ++i) {
        int lhs = account_id_prefix[i] - '0', rhs = adjust_num[i] - '0';
        encry_num += to_string((lhs + rhs) % 10);
    }

    std::string encry_bank_id = bank_id;
    if (solution == 2) {
        encry_bank_id = BIN[(g_bin_index_mp[bank_id] + 1) % BIN.size()];
    }
    encry_num  = encry_bank_id + encry_num + account_id_suffix + to_string(0);
    encry_num[encry_num.size() - 1] = '0' + calc_check_code(encry_num);
    return encry_num;
}

std::vector<std::string> decrypt(const std::string& credit_card_number, int solution = 1){
    if (!is_valid_credit_card_number(credit_card_number)) {
        printf("[ERROR] credit card number illegal, length or check sum fail!\n");
        return std::vector<std::string>();
    }

    int size = credit_card_number.size();
    int account_id_len = size - (BIN_LEN + 1);

    std::string bank_id = credit_card_number.substr(0, 6);
    std::string decry_bank_id = bank_id;
    if (solution == 2) {
        decry_bank_id = BIN[(g_bin_index_mp[bank_id] - 1 + BIN.size()) % BIN.size()];
    }

    std::string last_4_num = credit_card_number.substr(size - 4, 4);
    std::string adjust_num = decry_bank_id.substr(0, 2) + last_4_num;

    std::string account_id_prefix = credit_card_number.substr(6, 6);
    std::string account_id_suffix = credit_card_number.substr(12, account_id_len - 6);
    std::string decry_account_id_prefix;

    std::vector<std::string> ans;
    if (size >= 16) {
        for (int i = 0; i < adjust_num.size() - 1; ++i) {
            int decry_num = ((account_id_prefix[i] - '0') + 10 - (adjust_num[i] - '0')) % 10;
            decry_account_id_prefix += to_string(decry_num);
        }
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                if ((i + j) % 10 != (account_id_prefix.back() - '0')) {
                    continue;
                }
                std::string account_id = decry_account_id_prefix + to_string(i) + account_id_suffix;
                std::string number = decry_bank_id + account_id + to_string(j);
                if (is_valid_credit_card_number(number)) {
                    ans.emplace_back(number);
                }
            }
        }
    } else if (size == 13) {
        for (int i = 0; i < adjust_num.size() - 4; ++i) {
            int decry_num = ((account_id_prefix[i] - '0') + 10 - (adjust_num[i] - '0')) % 10;
            decry_account_id_prefix += to_string(decry_num);
        }

        for (int i9 = 0; i9 < 10; ++i9) {
            for (int ia = 0; ia < 10; ++ia) {
                if ((i9 + ia) % 10 != credit_card_number[8] - '0') continue;
                for (int ib = 0; ib < 10; ++ib) {
                    if ((ia + ib) % 10 != credit_card_number[9] - '0') continue;
                    for (int ic = 0; ic < 10; ++ic) {
                        if ((ib + ic) % 10 != credit_card_number[10] - '0') continue; 
                        for (int id = 0; id < 10; ++id) {
                            if ((ic + id) % 10 != credit_card_number[11] - '0') continue;
                            std::string account_id = decry_account_id_prefix + to_string(i9)
                                + to_string(ia) + to_string(ib) + to_string(ic) + account_id_suffix;
                            std::string number = decry_bank_id + account_id + to_string(id);
                            if (is_valid_credit_card_number(number)) {
                                ans.emplace_back(number);
                            }
                        }
                    }
                }
            }
        }
    } else if (size == 14) {
        for (int i = 0; i < adjust_num.size() - 4; ++i) {
            int decry_num = ((account_id_prefix[i] - '0') + 10 - (adjust_num[i] - '0')) % 10;
            decry_account_id_prefix += to_string(decry_num);
        }

        for (int i9 = 0; i9 < 10; ++i9) {
            for (int ia = 0; ia < 10; ++ia) {
                for (int ib = 0; ib < 10; ++ib) {
                    for (int ic = 0; ic < 10; ++ic) {
                        for (int id = 0; id < 10; ++id) {
                            if ((i9 + ib) % 10 == credit_card_number[8] - '0'  &&
                                (ia + ic) % 10 == credit_card_number[9] - '0' &&
                                (ib + credit_card_number[12] - '0') % 10 == credit_card_number[10] - '0' &&
                                (ic + id) % 10 == credit_card_number[11] - '0') {
                                std::string account_id = decry_account_id_prefix + to_string(i9)
                                    + to_string(ia) + to_string(ib) + to_string(ic) + account_id_suffix;
                                std::string number = decry_bank_id + account_id + to_string(id);
                                if (is_valid_credit_card_number(number)) {
                                    ans.emplace_back(number);
                                }
                            }
                        }
                    }
                }
            }
        }
    }  else if (size == 15) {
        for (int i = 0; i < adjust_num.size() - 4; ++i) {
            int decry_num = ((account_id_prefix[i] - '0') + 10 - (adjust_num[i] - '0')) % 10;
            decry_account_id_prefix += to_string(decry_num);
        }

        for (int i9 = 0; i9 < 10; ++i9) {
            for (int ia = 0; ia < 10; ++ia) {
                for (int ib = 0; ib < 10; ++ib) {
                    for (int ic = 0; ic < 10; ++ic) {
                        for (int id = 0; id < 10; ++id) {
                            if ((i9 + ic) % 10 == credit_card_number[8] - '0'  &&
                                (ia + credit_card_number[12] - '0') % 10 == credit_card_number[9] - '0' &&
                                (ib + credit_card_number[13] - '0') % 10 == credit_card_number[10] - '0' &&
                                (ic + id) % 10 == credit_card_number[11] - '0') {
                                std::string account_id = decry_account_id_prefix + to_string(i9)
                                    + to_string(ia) + to_string(ib) + to_string(ic) + account_id_suffix;
                                std::string number = decry_bank_id + account_id + to_string(id);
                                if (is_valid_credit_card_number(number)) {
                                    ans.emplace_back(number);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return ans;
}

int main(int argc, const char* argv[]) {
    for (int i = 0; i < BIN.size(); ++i) {
        g_bin_index_mp.emplace(BIN[i], i);
    }
    std::cout << "============================================" << std::endl;
    while (true) {
        int num = 0;
        std::cout << "please choose -1(exit) | 0(encrypt) | 1(decrypt): ";
        std::cin >> num;
        if (cin.fail()) {
            std::cout << "Error, pleadse input valid num: -1|0|1!" << std::endl;
            cin.clear();
            cin.sync();
        }
        if (num == -1) {
            break;
        }
        if (num == 0) {
            std::string credit_card_number;
            std::cout << "input number to be encrypt: ";
            std::cin >> credit_card_number;
            int solution = 0;
            std::cout << "input encrypt solution(1|2): ";
            std::cin >> solution;
            std::string encrypted_num = encrypt(credit_card_number, solution);
            if (encrypted_num.size() > 0) {
                std::cout << "encrypted:" << encrypted_num << std::endl;
            }
        } else if (num == 1) {
            std::string credit_card_number;
            std::cout << "input number to be decrypt: ";
            std::cin >> credit_card_number;
            int solution = 0;
            std::cout << "input decrypt solution(1|2): ";
            std::cin >> solution;
            auto decrypted_nums = decrypt(credit_card_number, solution);
            for (int i = 0; i < decrypted_nums.size(); ++i) {
                std::cout << "decrypted:" << decrypted_nums[i] << std::endl;
            }
        } else {
            std::cout << "error, input must be -1|0|1" << std::endl;
        }
    }
    return 0;
}
