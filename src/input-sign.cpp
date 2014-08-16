/*
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include "precompile.hpp"
#include <sx/command/input-sign.hpp>

#include <iostream>
#include <cstdint>
#include <bitcoin/bitcoin.hpp>
#include <sx/serializer/base16.hpp>
#include <sx/utility/utility.hpp>

using namespace bc;
using namespace sx;
using namespace sx::extension;
using namespace sx::serializer;

console_result input_sign::invoke(std::ostream& output, std::ostream& error)
{
    // Bound parameters.
    const auto index = get_index_option();
    const auto hash_type = get_sighash_option();
    const data_chunk& nonce = get_nonce_argument();
    const tx_type& tx = get_transaction_argument();
    const auto& secret = get_ec_private_key_argument();
    const auto& script = get_prevout_script_argument();

    if (nonce.size() < minimum_seed_size)
    {
        error << SX_INPUT_SIGN_SHORT_NONCE << std::endl;
        return console_result::failure;
    }

    if (tx.inputs.size() < index)
    {
        error << SX_INPUT_SIGN_INDEX_OUT_OF_RANGE << std::endl;
        return console_result::failure;
    }

    base16 signature;
    auto& buffer = signature.data();
    if (!sign_transaction(buffer, tx, index, script, secret, nonce, hash_type))
    {
        error << SX_INPUT_SIGN_INDEX_OUT_OF_RANGE << std::endl;
        return console_result::failure;
    }

    output << signature << std::endl;
    return console_result::okay;
}
