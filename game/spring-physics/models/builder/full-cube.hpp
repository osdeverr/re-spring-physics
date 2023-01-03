#pragma once

#include "model-builder.hpp"

namespace ModelBuilder {

    struct FullCube {
        union {
            struct {
                struct {
                    struct {
                        int z_pos;
                        int z_neg;
                    } y_pos;
                    struct {
                        int z_pos;
                        int z_neg;
                    } y_neg;
                } x_pos;
                struct {
                    struct {
                        int z_pos;
                        int z_neg;
                    } y_pos;
                    struct {
                        int z_pos;
                        int z_neg;
                    } y_neg;
                } x_neg;
            };
            int vertices[8];
        };
    };

    FullCube build_cube(Builder& builder, Vec3f center, Vec3f size);

}