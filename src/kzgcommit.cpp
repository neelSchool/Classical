#include "kzgcommit.hpp"
#include <iostream>
using namespace mcl::bn;
CommitmentKey KGen(size_t d, Fr&alpha_out) {
    CommitmentKey ck;
    Fr alpha;
    alpha.setByCSPRNG();
    alpha_out = alpha;
    G1 g;
    G2 h;
    mapToG1(g,1);
    mapToG2(h,1);
    ck.g_powers.resize(d+1);
    ck.g_powers[0] = g;
    Fr power = alpha;
    for (size_t i = 1; i <= d; i++) {
        G1::mul(ck.g_powers[i],g,power);
        power *= alpha;
    }
    ck.h = h;
    G2::mul(ck.h_alpha,h,alpha);
    return ck;
}
CommitmentKey Update(const CommitmentKey& ck, Fr& beta_out, UpdateProof& proof) {
    Fr beta;
    beta.setByCSPRNG();
    beta_out = beta;
    CommitmentKey ck_up;
    ck_up.g_powers.resize(ck.g_powers.size());
    for (size_t i = 0; i < ck.g_powers.size(); i++) {
        G1::mul(ck_up.g_powers[i], ck.g_powers[i], beta);
    }
    G2::mul(ck_up.h, ck.h, beta);
    G2::mul(ck_up.h_alpha, ck.h_alpha, beta);
    mapToG1(proof.g_beta, 1);
    G1::mul(proof.g_beta, proof.g_beta, beta);
    mapToG2(proof.h_beta, 1);
    G2::mul(proof.h_beta, proof.h_beta, beta);
    return ck_up;
}
bool VerifyUpdate(const CommitmentKey& old_ck, const CommitmentKey& new_ck, const UpdateProof& proof) {
    GT lhs, rhs;
    pairing(lhs, new_ck.g_powers[1], old_ck.h);
    pairing(rhs, old_ck.g_powers[1], proof.h_beta);
    return lhs == rhs;
}
G1 Commit(const std::vector<Fr>& coeffs, const CommitmentKey& ck) {
    G1 c;
    c.clear();
    for (size_t i = 0; i < coeffs.size(); i++) {
        G1 term;
        G1::mul(term, ck.g_powers[i], coeffs[i]);
        G1::add(c, c, term);
    }
    return c;
}
G1 EvalProof(const std::vector<Fr>& coeffs, const Fr& y, const Fr& z, const CommitmentKey& ck) {
    // q(x) = (f(x) - y) / (x - z)
    std::vector<Fr> q_coeffs = divByXminusZ(coeffs, z, y);
    return Commit(q_coeffs, ck);
}
bool Check(const G1& commitment, const G1& proof, const Fr& y, const Fr& z, const CommitmentKey& ck) {
    // Compute C - g^y
    G1 g;
    mapToG1(g, 1);
    G1 g_y;
    G1::mul(g_y, g, y);
    G1 lhs_g;
    G1::sub(lhs_g, commitment, g_y);

    GT lhs, rhs;
    pairing(lhs, lhs_g, ck.h); // e(C - g^y, h)

    // Compute h^{alpha - z} using additive notation: (alpha - z)H = alpha H + (-z)H
    G2 h_term;
    G2::mul(h_term, ck.h, z);      // z * h
    G2::neg(h_term, h_term);       // -z * h
    G2::add(h_term, h_term, ck.h_alpha); // h_alpha - z * h

    pairing(rhs, proof, h_term);   // e(pi, h^{alpha - z})
    return lhs == rhs;
}

std::vector<Fr> divByXminusZ(const std::vector<Fr>& coeffs, const Fr& z, const Fr& y) {
    // coeffs: a_0, a_1, ..., a_d  (low degree to high degree)
    // We want q such that (x - z) * q(x) = f(x) - y

    const size_t d = coeffs.size() - 1; // degree of f
    std::vector<Fr> q(d);               // degree d-1

    // Build coefficients in high-to-low order for synthetic division.
    // a_high[0] = a_d, ..., a_high[d] = a_0 - y.
    std::vector<Fr> a_high(d + 1);
    for (size_t i = 0; i < d; i++) {
        a_high[i] = coeffs[d - i]; // a_d, a_{d-1}, ..., a_1
    }
    a_high[d] = coeffs[0] - y; // constant term adjusted by -y

    // Synthetic division in high-to-low form.
    std::vector<Fr> b_high(d + 1);
    b_high[0] = a_high[0];
    for (size_t i = 1; i <= d; i++) {
        b_high[i] = a_high[i] + z * b_high[i - 1];
    }

    // b_high[d] is the remainder; for a correct y, it should be 0.
    // Quotient coefficients in high-to-low: b_high[0..d-1].

    // Convert quotient back to low-to-high order.
    for (size_t i = 0; i < d; i++) {
        q[i] = b_high[d - 1 - i];
    }

    return q;
}

