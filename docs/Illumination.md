# Illumination

## Light direction

The light direction is computed by mapping the cursor position in normalized device coordinates (NDC) to the half unit sphere $S_h^2$:

$$
S_h^2 = \{(x, y, z) \in S^2 | z \ge 0 \}
$$

This mapping is done in the following way.