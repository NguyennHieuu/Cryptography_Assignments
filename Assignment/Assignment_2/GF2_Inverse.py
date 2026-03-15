def get_degree(poly):
    # Bậc của đa thức = vị trí bit cao nhất
    if poly == 0:
        return -1
    return poly.bit_length() - 1


def poly_to_str(poly):
    # Biểu diễn đa thức dạng x^i + ...
    if poly == 0:
        return "0"
    terms = []
    for i in range(get_degree(poly), -1, -1):
        if poly & (1 << i):
            if i == 0:   terms.append("1")
            elif i == 1: terms.append("x")
            else:        terms.append(f"x^{i}")
    return " + ".join(terms)


def poly_divmod(a, b):
    # Chia đa thức trong GF(2), trả về (thương q, dư r)
    if b == 0:
        raise ZeroDivisionError("Không thể chia cho 0")
    q = 0
    r = a
    deg_b = get_degree(b)
    while True:
        deg_r = get_degree(r)
        if deg_r < deg_b:
            break
        shift = deg_r - deg_b
        q ^= (1 << shift)
        r ^= (b << shift)
    return q, r


def poly_mul(a, b):
    # Nhân đa thức trong GF(2)
    res = 0
    while b > 0:
        if b & 1:
            res ^= a
        a <<= 1
        b >>= 1
    return res


def extended_euclidean_gf2(a, m, verbose=True):
    # Tìm x sao cho a(x) * x ≡ 1 (mod m(x))
    r_prev, r_curr = m, a
    x_prev, x_curr = 0, 1

    if verbose:
        W = 14
        header = f"{'Bước':^5} | {'q (thương)':^{W}} | {'r (số dư)':^{W}} | {'x_i':^{W}}"
        print(header)
        print("-" * len(header))
        print(f"{'−1':^5} | {'—':^{W}} | {r_prev:^{W}} | {x_prev:^{W}}")
        print(f"{'0':^5} | {'—':^{W}} | {r_curr:^{W}} | {x_curr:^{W}}")

    step = 1
    while True:
        q, r_next = poly_divmod(r_prev, r_curr)
        x_next = x_prev ^ poly_mul(q, x_curr)

        if verbose:
            print(f"{step:^5} | {q:^{W}} | {r_next:^{W}} | {x_next:^{W}}")

        if r_next == 0:
            break

        r_prev, r_curr = r_curr, r_next
        x_prev, x_curr = x_curr, x_next
        step += 1

    if r_curr != 1:
        return None
    return x_curr


# m(x) = x^10 + x^3 + 1 = 1033
m_poly = (1 << 10) | (1 << 3) | 1

print(f"Đa thức tối giản: m(x) = {poly_to_str(m_poly)}  (thập phân: {m_poly})")
print(f"Trường: GF(2^10)\n")

test_vectors = [523, 1015]

for val in test_vectors:
    print(f"\n{'═'*60}")
    print(f" Tìm nghịch đảo của a = {val}  [a(x) = {poly_to_str(val)}]")
    print(f"{'═'*60}")

    inv = extended_euclidean_gf2(val, m_poly, verbose=True)

    if inv is not None:
        check = poly_divmod(poly_mul(val, inv), m_poly)[1]
        print(f"\n  Nghịch đảo:  {val}^(-1) = {inv}  [{poly_to_str(inv)}]")
        print(f"  Kiểm tra:    ({val} × {inv}) mod {m_poly} = {check}")
    else:
        print(f"\n  Không có nghịch đảo (gcd ≠ 1)")