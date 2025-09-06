<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

use App\Models\AccessZones;

class AccessZonesSeeder extends Seeder
{
    public function run(): void
    {
        $data = [
            'entity' => 0,
            'access_zone' => 2,
        ];
        foreach(range(1, 10) as $i)
        {
            $data['entity'] = $i;
            AccessZones::insert($data);
        }
    }
}
