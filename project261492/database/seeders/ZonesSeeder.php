<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

use App\Models\Zones;

class ZonesSeeder extends Seeder
{
    public function run(): void
    {
        $data = [
            [
                'description' => '261491',
                'location' => '491',
            ],
            [
                'description' => '261492',
                'location' => '492',
            ],
        ];
        Zones::insert($data);
    }
}
