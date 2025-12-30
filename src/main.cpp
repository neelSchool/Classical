#include "kzgcommit.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace mcl::bn;
int main() {
    initPairing(mcl::BLS12_381); //pairing initialization
    Fr alpha;
    CommitmentKey ck = KGen(5,alpha); //key gen
    // Fr beta;
    // UpdateProof proof;
    // CommitmentKey ck_up = Update(ck,beta,proof); //key upd
    std::vector<Fr> coeffs = {Fr(3),Fr(2),Fr(1)}; // 3 + 2x + x^2
    Fr z = Fr(1);
    Fr y = Fr(6); // f(1) = 6
    G1 commitment = Commit(coeffs, ck); //up
    G1 eval_proof = EvalProof(coeffs, y, z, ck); //up
    bool valid = Check(commitment, eval_proof, y, z, ck); //ckup
    if (valid) {
        std::cout << "Commitment and evaluation proof verified!" << std::endl;
    } else {
        std::cout << "Verification failed!" << std::endl;
    }
    std::vector<Fr> q = divByXminusZ(coeffs, z, y);
    std::cout << "q[0], q[1] = " << q[0].getStr(10) << ", " << q[1].getStr(10) << std::endl;

    return 0;
}