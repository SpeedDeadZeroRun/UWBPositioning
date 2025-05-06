<?php

namespace Database\Seeders;

use Illuminate\Database\Console\Seeds\WithoutModelEvents;
use Illuminate\Database\Seeder;

use App\Models\EntityTypes;

class EntityTypesSeeder extends Seeder
{
    public function run(): void
    {
        $data = [
            [
                'type' => 'admin'
            ],
            [
                'type' => 'user'
            ],
            [
                'type' => 'controller'
            ],
            [
                'type' => 'anchor'
            ],
            [
                'type' => 'tag'
            ],
        ];
        EntityTypes::insert($data);
    }
}
