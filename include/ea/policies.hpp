#pragma once

#include <cstddef>
#include <cstdint>

#include <ea/config.hpp>

namespace ea::policy {

    namespace sum {
        struct naive {};
        struct kahan {};
        struct neumaier {};
        struct pairwise {};
        struct pairwise_with_compensation {};
        struct auto_select {};

    } // namespace sum


    namespace dot {
        struct naive {};
        struct pairwise {};
        struct pairwise_with_compensation {};
        struct auto_select {};
    } // namespace dot


    namespace poly {
        struct horner {};
        struct compensated_horner {};
        struct auto_select {};
    } // namespace poly


    namespace solve {
        struct lu_with_pivoting {};
        struct qr {};
        struct cholesky {};
        struct auto_select {};
    } // namespace solve


    namespace refine {
        struct none {};
        struct iterative_refinement {
            std::size_t max_iters = 5;
            double      tol = 0.0;
        };

    } // namespace refine


    namespace jitter {
        struct none {};
        struct diagonal {
            double lambda_factor = 1e-12;
        };
    } // namespace jitter


    namespace chain {
        struct auto_select {
        };

    } // namespace chain

} // namespace ea::policy


namespace ea {
    namespace sum_policy    = policy::sum;
    namespace dot_policy    = policy::dot;
    namespace poly_policy   = policy::poly;
    namespace solve_policy  = policy::solve;
    namespace refine_policy = policy::refine;
    namespace jitter_policy = policy::jitter;
    namespace chain_policy  = policy::chain;
} // namespace ea
