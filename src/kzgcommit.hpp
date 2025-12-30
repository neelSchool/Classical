#ifndef KZG_COMMITMENT_HPP
#define KZG_COMMITMENT_HPP

#include <mcl/bn.hpp>
#include <vector>

using namespace mcl::bn;

struct CommitmentKey {
    std::vector<G1> g_powers;  // g^(α^i), i = 0..D
    G2 h;
    G2 h_alpha;
};

struct UpdateProof {
    G1 g_beta;
    G2 h_beta;
};

CommitmentKey KGen(size_t D, Fr& alpha_out);
CommitmentKey Update(const CommitmentKey& ck, Fr& beta_out, UpdateProof& proof);
bool VerifyUpdate(const CommitmentKey& old_ck, const CommitmentKey& new_ck, const UpdateProof& proof);
G1 Commit(const std::vector<Fr>& coeffs, const CommitmentKey& ck);
G1 EvalProof(const std::vector<Fr>& coeffs, const Fr& y, const Fr& z, const CommitmentKey& ck);
std::vector<Fr> divByXminusZ(const std::vector<Fr>& coeffs, const Fr& z, const Fr& y);
bool Check(const G1& commitment, const G1& proof, const Fr& y, const Fr& z, const CommitmentKey& ck);

#endif