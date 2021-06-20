#include "tuple.h"

Tuple::Tuple(const Tuple& tuple_in) {
    for (int index = 0; index < tuple_in.data_.size(); index++)
    {
        this->data_.push_back(tuple_in.data_[index]);
    }
}

inline int Tuple::getSize() {
    return (int)data_.size();
}

//��������
void Tuple::addData(Data data_in) {
    this->data_.push_back(data_in);
}

//�õ�Ԫ���е�����
vector<Data> Tuple::getData() const {
    return this->data_;
}

void Tuple::showTuple() {
    for (int index = 0; index < getSize(); index++) {
        if (data_[index].type == -1)
            cout << data_[index].datai << '\t';
        else if (data_[index].type == 0)
            cout << data_[index].dataf << '\t';
        else
            cout << data_[index].datas << '\t';
    }
    cout << std::endl;
}

